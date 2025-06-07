@echo off
git add .
git commit -m "Auto-sync" || exit /b 0  REM Exit if nothing to commit
git push
REM