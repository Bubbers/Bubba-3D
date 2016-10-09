#!/bin/bash

if [[ "${TRAVIS_BRANCH}" != "master" ]]
then
    echo "Branch ${TRAVIS_BRANCH} is not master"
    exit 0
fi

function teardown {
    git checkout master
}

git checkout gh-pages
BRANCH_CHECKOUT=$?

if [[  "${BRANCH_CHECKOUT}" != "0" ]]
then
    echo "Failed to switch to branch 'gh-pages', aborting..."
    exit 1
fi

cp -r build/docs/html/* .
COPY_DOCS=$?

if [[ "${COPY_DOCS}" != "0" ]]
then
    echo "Filed to copy docs, aborting..."
    teardown
    exit 1
fi

git add .
git commit -m "Built new docs on master"
git push "git@github.com:Bubbers/Bubba-3D.git" gh-pages -f
PAGES_PUSH=$?

if [[ "${PAGES_PUSH}" != "0" ]]
then
    echo "Failed to push gh-pages to github"
    teardown
    exit 1
fi

teardown
