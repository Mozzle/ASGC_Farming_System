Write-Output "Performing git working tree setup. This may take a minute..."

git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\ASGC_Farming_System.ioc
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\.project
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\.mxproject
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\CM7\.cproject
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\CM7\.project
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\CM4\.cproject
git update-index --skip-worktree .\NUCLEO-H755ZI\ASGC_Farming_System\CM4\.project
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM4/.settings/" | ForEach-Object { git update-index --skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM7/.settings/" | ForEach-Object { git update-index --skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM4/Debug/" | ForEach-Object { git update-index --skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM7/Debug/" | ForEach-Object { git update-index --skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM7/Debug/" | ForEach-Object { git update-index --skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM7/Core" | ForEach-Object { git update-index --no-skip-worktree $_ }
git ls-files "NUCLEO-H755ZI/ASGC_Farming_System/CM4/Core" | ForEach-Object { git update-index --no-skip-worktree $_ }

Write-Output "git worktree setup complete. You can now exit this window."

pause
