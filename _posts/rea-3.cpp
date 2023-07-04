enum TermKind {
  Term_Union,
  Term_Constructor,
  Term_Function,
  Term_Fork,
  Term_Variable,
  Term_Composite,
  Term_Arrow,
  Term_Pointer,
};

struct Term {
  TermKind  kind;
  i32       serial;
  Term     *type;
  Token    *global_name;
};

enum PointerKind
{
  Pointer_Stack,
  Pointer_Heap  // we don't need to care about this yet!
};

struct Variable : Term {
  String name;
  i32    delta;
  i32    index;
};

struct StackPointer {
  String name;
  i32    depth;
  i32    index;
  Term  *value;
};

sstruct Pointer : Term {
  Record      *ref;
  PointerKind  pointer_kind;
  StackPointer stack;
};

struct LocalBinding
{
  i32           hash;
  String        key;
  i32           var_index;
  LocalBinding *tail;
};

struct LocalBindings {
  Arena         *arena;
  LocalBinding   table[128];
  LocalBindings *tail;
};

struct Scope {
  Scope    *outer;
  i32       depth;
  i32       param_count;
  Pointer **pointers;
};

struct Typer {
  LocalBindings *bindings;
  Scope         *scope;
};

internal Term *
buildComposite(Typer *typer, CompositeAst *in, Term *goal)
{
  // hidden code: we don't care about this yet
}

struct LookupCurrentFrame { LocalBinding* slot; b32 found; };

internal LookupCurrentFrame
lookupCurrentFrame(LocalBindings *bindings, String key, b32 add_if_missing) {
  // hidden code, basically lookup the key in bindings->table

  // This part is omitted partly because it does a hash table lookup,
  // and I have no idea why I did that instead of just a linear search...
}

inline Term *
lookupLocalName(Typer *typer, Token *token)
{
  String name = token->string;
  LocalBindings *bindings = typer->bindings;
  Term *out = {};
  Scope *scope = typer->scope;
  for (i32 stack_delta = 0; bindings; stack_delta++)
  {
    LookupCurrentFrame lookup = lookupCurrentFrame(bindings, name, false);
    if (lookup.found)
    {
      assert(lookup.slot->var_index < scope->param_count);
      out = scope->pointers[lookup.slot->var_index];
      break;
    }
    else
    {
      bindings = bindings->tail;
      scope    = scope->outer;
    }
  }
  return out;
}

inline Pointer *
newStackPointer(String name, i32 depth, i32 index, Term *type)
{
  assert(depth);
  Pointer *out = newTerm(temp_arena, Pointer, type);
  out->pointer_kind = Pointer_Stack;
  out->stack.name   = name;
  out->stack.depth  = depth;
  out->stack.index  = index;
  return out;
}

// This function just recursively turn stack pointers back to variables
inline Term *
toAbstractTerm_(AbstractContext *ctx, Term *in0)
{
  assert(ctx->zero_depth >= ctx->env_depth);
  Term *out0 = 0;
  Arena *arena = ctx->arena;

  if (isGlobalValue(in0))
  {
    out0 = in0;
  }
  else
  {
    switch (in0->kind)
    {
      case Term_Pointer:
      {
        Pointer *in = castTerm(in0, Pointer);
        if (in->pointer_kind == Pointer_Stack)
        {
          if (in->stack.depth > ctx->env_depth)
          {
            Variable *out = newTerm(arena, Variable, 0);
            out->name  = in->stack.name;
            out->delta = ctx->zero_depth - in->stack.depth;
            out->index = in->stack.index;
            assert(out->delta >= 0);
            out0 = out;
          }
          else
          {
            out0 = in0;
          }
        }
      } break;

      case Term_Arrow:
      {
        Arrow *in  = castTerm(in0, Arrow);
        Arrow *out = copyTerm(arena, in);
        ctx->zero_depth++;
        allocateArray(arena, in->param_count, out->param_types);
        for (i32 i=0; i < in->param_count; i++)
        {
          out->param_types[i] = toAbstractTerm_(ctx, in->param_types[i]);
        }
        out->output_type = toAbstractTerm_(ctx, in->output_type);
        ctx->zero_depth--;
        out0 = out;
      } break;

      case Term_Fork:
      {
        Fork *in  = castTerm(in0, Fork);
        Fork *out = copyTerm(arena, in);
        out->subject = toAbstractTerm_(ctx, in->subject);
        out->cases  = pushArray(arena, in->case_count, Term*);
        for (i32 i=0; i < in->case_count; i++)
        {
          out->cases[i] = toAbstractTerm_(ctx, in->cases[i]);
        }
        out0 = out;
      } break;

      case Term_Function:
      {
        Function *in  = castTerm(in0, Function);
        Function *out = copyTerm(arena, in);
        ctx->zero_depth++;
        out->body = toAbstractTerm_(ctx, in->body);
        ctx->zero_depth--;
        out0 = out;
      } break;

      case Term_Variable:
      {
        out0 = copyTerm(arena, (Variable *)in0);
      } break;
    }

    if (out0 != in0)
    {
      out0->type = toAbstractTerm_(ctx, in0->type);
    }
  }
  assert(out0);

  return out0;
}

inline void
addLocalBinding(Typer *typer, String key, i32 var_index)
{
  auto lookup = lookupCurrentFrame(typer->bindings, key, true);
  lookup.slot->var_index = var_index;
}

inline Scope *
introduceSignature(Scope *outer_scope, Arrow *signature)
{
  i32 param_count = signature->param_count;
  Scope *scope = newScope(outer_scope, param_count);
  for (i32 param_i=0; param_i < param_count; param_i++)
  {
    String  name = signature->param_names[param_i];
    Term   *type = toValue(scope, signature->param_types[param_i]);
    // :fixpoint-pointers
    scope->pointers[param_i] = newStackPointer(name, scope->depth, param_i, type);
  }
  return scope;
}

inline Term *
toValue(Scope *scope, Term *in0)
{
  // hidden code: let's just say this doesn't do anything for now.
}

inline LocalBindings *
extendBindings(Typer *typer)
{
  LocalBindings *out = pushStruct(temp_arena, LocalBindings, true);
  out->tail  = typer->bindings;
  out->arena = temp_arena;
  typer->bindings = out;
  return out;
}

inline Function *
buildFunctionGivenSignature(Typer *typer0, Arrow *signature, Ast *in_body,
                            Token *global_name=0, FunctionAst *in_fun=0)
{
  i32 unused_var serial = DEBUG_SERIAL;
  // :persistent-global-function.

  // NOTE: We MUST control the arena, otherwise the self-reference will be
  // invalidated (TODO: maybe the copier should know about self-reference).
  Arena *arena = global_name ? global_state.top_level_arena : temp_arena;

  Function *out = newTerm(arena, Function, signature);
  if (in_fun) out->function_flags = in_fun->function_flags;

  Term *body = 0;
  Typer typer_ = *typer0;
  {
    Typer *typer = &typer_;
    typer->scope = introduceSignature(typer->scope, signature);
    extendBindings(typer);

    if (noError())
    {
      for (i32 index=0; index < signature->param_count; index++)
      {
        String name = signature->param_names[index];
        addLocalBinding(typer, name, index);
      }
      Term *body_type = toValue(typer->scope, signature->output_type);
      if ((body = buildTerm(typer, in_body, body_type)))
      {
        if (signature->output_type == hole)
        {
          // write back the output type
          signature->output_type = toAbstractTerm(temp_arena, body->type, getScopeDepth(typer0));
          Term *check = toValue(typer->scope, signature->output_type);
          assertEqualNorm(body->type, check);
        }
      }
    }
  }

  if (body)
  {
    out->body = toAbstractTerm(temp_arena, body, getScopeDepth(typer0));
    if (global_name)
    {
      out->body = copyToGlobalArena(out->body);
    }
  }
  NULL_WHEN_ERROR(out);
  return out;
}

inline b32
matchType(Term *actual, Term *goal)
{
  // hidden code: just pretend that it's deep-comparing the terms
  // but in reality we have to normalize the terms first and compare them
}

forward_declare internal BuildTerm
buildTerm(Typer *typer, Ast *in0, Term *goal0)
{
  // beware: Usually we mutate in-place, but we may also allocate anew.
  i32 UNUSED_VAR serial = DEBUG_SERIAL++;

  assert(goal0);
  Arena *arena = temp_arena;
  Term *value = 0;
  b32 should_check_type = true;
  b32 recursed = false;

  switch (in0->kind)
  {
    case Ast_CompositeAst:
    {
      value = buildComposite(typer, (CompositeAst*)in0, goal0);
    } break;

    case Ast_Identifier:
    {
      Identifier *in = castAst(in0, Identifier);
      Token *name = &in->token;
      if (Term *v = lookupLocalName(typer, name))
      {
        value = v;
      }
      else
      {
        if (GlobalSlot *slot = lookupGlobalName(name))
        {
          for (i32 value_id = 0; value_id < slotCount(slot); value_id++)
          {
            Term *slot_value = slot->terms[value_id];
            if (matchType((slot_value)->type, goal0))
            {
              value = slot_value;
            }
          }
        }

        if (value)
        {
          should_check_type = false;
        }
      }
    } break;

    case Ast_ArrowAst:
    {
      ArrowAst *in = (ArrowAst *)(in0);
      Arrow *out = newTerm(arena, Arrow, rea.Type);
      i32 param_count = in->param_count;

      // :arrow-copy-done-later
      out->param_count = param_count;
      out->param_names = in->param_names;
      out->param_flags = in->param_flags;

      if (noError())
      {
        Scope *scope = typer->scope = newScope(typer->scope, param_count);
        extendBindings(typer);
        out->param_types = pushArray(arena, param_count, Term*);
        for (i32 i=0; i < param_count && noError(); i++)
        {
          if (Term *param_type = buildTerm(typer, in->param_types[i], hole))
          {
            out->param_types[i] = toAbstractTerm(arena, param_type, getScopeDepth(scope->outer));
            String name = in->param_names[i];
            scope->pointers[i] = newStackPointer(name, scope->depth, i, param_type);
            addLocalBinding(typer, name, i);
          }
        }

        if (noError())
        {
          value = out;
          // :no-output-type
          if (in->output_type)
          {
            if (Term *output_type = buildTerm(typer, in->output_type, hole))
            {
              out->output_type = toAbstractTerm(arena, output_type, getScopeDepth(scope->outer));
            }
          }
          else
          {
            out->output_type = hole;
          }
        }
        unwindBindingsAndScope(typer);
      }
    } break;

    case Ast_FunctionAst:
    {
      FunctionAst *in = castAst(in0, FunctionAst);
      Term *type = goal0;
      if (in->signature)
      {
        if (isNameOnlyArrowType(in->signature))
        {
          type = buildNameOnlyArrowType(typer, in->signature, goal0);
        }
        else
        {
          type = buildTerm(typer, in->signature, hole);
        }
      }

      if (noError())
      {
        if (Arrow *signature = castTerm(type, Arrow))
        {
          value = buildFunctionGivenSignature(typer, signature, in->body, 0, in);
        }
        else
        {
          reportError(in0, "function signature is required to be an arrow type");
          attach("type", type);
        }
      }
    } break;

    case Ast_ForkAst:
    {
      ForkAst *fork = castAst(in0, ForkAst);
      value = buildFork(typer, fork, goal0);
      recursed = true;
    } break;
  }

  if (noError())
  {// typecheck if needed
    Term *actual = value->type;
    if (should_check_type && !recursed)
    {
      if (!matchType(actual, goal0))
      {
        if (expectedWrongType(typer)) silentError();
        else
        {
          reportError(in0, "actual type differs from expected type");
          attach("got", actual);
        }
      }
    }
  }

  return BuildTerm{.value=value};
}
