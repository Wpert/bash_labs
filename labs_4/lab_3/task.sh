# if i want to see the graph of the repo
# git log --graph --all --name-only --oneline

dirname="testdir"

if test -d $dirname; then
  rm -rf $dirname
fi


mkdir dirname
cd dirname

git init
git commit --allow-empty -m init

mkdir "lib"
cd lib
touch mylib.c
cd ..

git add lib/mylib.c
git commit -m 'create lib/mylib.c'

git branch new

touch main.c
git add main.c
git commit -m 'create main.c'

git checkout new

touch featureA.c
git add featureA.c
git commit -m 'create featureA.c'

touch featureB.c
git add featureB.c
git commit -m 'create featureB.c'

git checkout master

git merge new -m merge

git branch merged

git reset --hard master@{1}

git log --graph --all --name-only --oneline
# HEAD is now at f26d28e create main.c
# *   3a5cff1 (merged) merge
# |\  
# | * 915dcbf (new) create featureB.c
# | | featureB.c
# | * d07babb create featureA.c
# | | featureA.c
# * | f26d28e (HEAD -> master) create main.c
# |/  
# |   main.c
# * b1dbf4d create lib/mylib.c
# | lib/mylib.c
# * d87d6f5 init
