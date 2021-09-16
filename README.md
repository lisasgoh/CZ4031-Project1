# Compilation into `main.exe`

`g++ -o main.exe memory-pool.h memory-pool.cpp b-plus-tree.cpp main.cpp`

# Running `main.exe`

`./main.exe`

# Workflow
git stash
git checkout main && git pull
git checkout -b feature/XXX
git stash pop // there might be conflict here, resolve carefully. If unsure, always resolve in favour of master (ditch incoming changes)
// work on your part
git commit -m "XXX"
git push // will prompt you to set upstream branch, do it
// open PR for your branch
// ping everyone iff all checks pass (all green)

// If you get a merge conflict (GitHub will tell you, red cross will appear)
git stash // only if you've local changes you still want to preserve
git checkout main && git pull
git checkout feature/XXX
git rebase -i main // place your changes ON TOP OF master branch
// pick all changes - in nano, just Ctrl-O, Enter, Ctrl-X
// rebase will stop as soon as merge conflict is detected. resolve it
// once rebase process is completed, make sure everything works locally
git branch // ensure you're on the feature branch that you just rebased against main
git push -f
