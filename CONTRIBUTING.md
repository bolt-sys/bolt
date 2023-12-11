# Bolt

## Table of Contents

- [Bolt](#bolt)
    - [Table of Contents](#table-of-contents)
    - [How to Contribute](#how-to-contribute)
    - [Reporting Bugs](#reporting-bugs)
    - [Commit Messages](#commit-messages)
    - [Pull Requests](#pull-requests)
    - [Coding Style](#coding-style)

## How to Contribute

If you want to contribute to this project,
you can look at the [Trello](https://trello.com/b/SSaAl6cE/bolt-os) board to see what needs to be done.
If you want to contribute something that isn't on the board, feel free to do so.
Just make sure to follow the [Coding Style](#coding-style)
and to create a [Pull Request](#pull-requests) with your changes.

1. Fork the repository
2. Create a new branch
3. Make your changes
4. Commit your changes
5. Push your changes
6. Create a pull request
7. Wait for a review
8. If your pull request gets accepted,
   your changes will be merged into the main branch and will be available in the next release
9. If your pull request gets rejected,
   you can either try to fix the issues or you can create a new pull request with your changes

## Reporting Bugs

We're using [GitHub Issues](https://github.com/bolt-sys/bolt/issues) to track bugs.
Please make sure to include the following information in your bug report:

- What did you do?
- What did you expect to happen?
- What happened instead?
- How can we reproduce the issue?
- Logs, screenshots, etc.
  Please make sure that your bug report is easy to understand and that it is not a duplicate of an existing issue.

## Commit Messages

Clearly documenting your commits helps us keep the project history clean and easy to navigate.
To document your commits effectively, we ask that you adhere to the following guidelines:

### Commit Title

The title of your commit should clearly express the feature or fix that the commit adds.
Be concise and specific. An example of a good commit title is:

    "Add: Reallocate Memory Feature to the Memory Module"

Try to avoid generic commit titles like "Updated files" or "Minor fixes".
The goal of the title is to give a clear indication of the content and purpose of the commit.

### Commit Description

A description is entirely optional, but it is recommended for more complex commits.

### Commit generation

it is permitted to use `AI` to automatically generate your commit title, given that its kinda of
a pain to write a good commit, and that the commit title is not the most important part of the commit.
But please make sure to keep it relatively relevant to the commit. double check it before you commit it.

## Pull Requests

Your Pull Requests play a crucial part in the development of this Project,
and we appreciate the time and effort you put into them.

To ensure clarity and to facilitate the review process, we have a specific format for PR titles and descriptions:

### PR Title

The title of your Pull Request should clearly indicate what issue it resolves or what it adds
if it is not related to an existing issue. Be concise and specific. Examples of good PR titles are:

- "Fixes #123: Corrects Null Pointer Exception in Login Function"
- "Adds: New Logging Functionality to Improve Debugging"

The use of "Fixes #issue_number"  or "Adds" in your title helps us identify what the PR is related to quickly.

### PR Description

The description should provide a brief overview of the changes you have made and the reason behind them.
Detail what you have changed, why you changed it, and what impact it will have on the project.
This will help the reviewers understand your thought process and review your PR accurately.

Keep in mind to be descriptive yet succinct, and feel free to use bullet points to outline your changes.
Here is a suggested template for your PR description:

    "<Short introduction about the module/function affected and the main change>
    
     Detailed explanation of the important parts of the commit,
     focusing on the reasons behind the change and what they mean
     for the project. Try to group related changes, and clearly
     differentiate between different changes made in the commit.>"

Following these guidelines will help us maintain a clean, well-documented project history.
Thank you for your contribution!

## Coding Style

We're using a tool called [uncrustify](https://github.com/uncrustify/uncrustify) to format our code.
you can simply run `bazel run //:format` to format all the code in the project. although it is recommended to use
your IDE's integration with uncrustify to format the code as you write it.

or rather, we use a very specific fork of uncrustify by the [Project MU](https://microsoft.github.io/mu/) which has
prebuilt binaries available for Windows, Linux and macOS. you can find the binaries
[here](https://dev.azure.com/projectmu/Uncrustify/_artifacts/feed/mu_uncrustify).

## Navigation

To navigate around the codebase, it's recommended to
use [Sourcegraph](https://sourcegraph.com/github.com/bolt-sys/bolt)
or your IDE's search implementation.

## Code of Conduct

Please read our [Code of Conduct](CODE_OF_CONDUCT.md) before contributing to this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details \
TL;DR: Do whatever you want with it. Just don't blame me if it breaks your computer but credit me if you use it in your
project.
