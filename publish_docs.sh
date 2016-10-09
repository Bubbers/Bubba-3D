#!/bin/bash
set -e

if [[ "${TRAVIS_BRANCH}" != "master" ]]
then
    echo "Branch ${TRAVIS_BRANCH} is not master"
    exit 0
fi

# Fetch the gh-pages branch and switch to it
git fetch origin gh-pages
git checkout -qf FETCH_HEAD
git checkout -b gh-pages

# Setting git author
git config user.email "noreply@travis-ci.org"
git config user.name "Travis CI"

# Moving docs to root directory and committing
cp -r build/docs/html/* .
git add .

git commit -m "Built new docs on master"

# Adding deploy key
ENCRYPTION_LABEL="773359cabcfc"
ENCRYPTED_KEY_VAR="encrypted_${ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}

openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in deploy_key.enc -out deploy_key -d
chmod 600 deploy_key
eval `ssh-agent -s`
ssh-add deploy_key

# Pushing to github
git push "git@github.com:Bubbers/Bubba-3D.git" gh-pages -f
