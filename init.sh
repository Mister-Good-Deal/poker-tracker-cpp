#!/bin/bash

# Simply copy the hook in the git directory
if `ln -s hooks/pre-commit .git/hooks/pre-commit`; then
  echo "pre-commit hook enable in \`.git/hooks\` repository for coding style"
else
  echo "Fail to setup the git hook in \`.git/hooks\` repository for coding style"
fi

printf "\nInitialisation complete\n"
