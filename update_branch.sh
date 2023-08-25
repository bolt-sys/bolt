# bin/bash
git checkout main
git fetch -p origin
git merge origin/main
git checkout $1
git merge main
git push origin $1