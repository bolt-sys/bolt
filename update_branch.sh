# bin/bash
MAIN_BRANCH="main"
BRANCH=$(git branch --show-current)
if [ BRANCH == MAIN_BRANCH ]
then
    echo "You are on main branch, please checkout to another branch"
    exit 1
fi
if [ $1 == "release" ]
then
    git checkout $MAIN_BRANCH
    git merge $BRANCH
    git push origin $MAIN_BRANCH
    exit 1
fi
git checkout $MAIN_BRANCH
git fetch -p origin
git merge origin/$MAIN_BRANCH
git checkout BRANCH
git merge origin/$MAIN_BRANCH
git push origin BRANCH