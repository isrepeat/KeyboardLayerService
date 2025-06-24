@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

REM --- Navigate to current repo directory ---
CD /D "%~dp0"

REM --- Check if we are in a Git repository ---
git rev-parse --is-inside-work-tree >nul 2>&1
IF ERRORLEVEL 1 (
    ECHO "Error: This is not a Git repository."
    PAUSE
    EXIT /B
)

REM --- Check if GitHub CLI is installed ---
gh --version >nul 2>&1
IF ERRORLEVEL 1 (
    ECHO "GitHub CLI (gh) is not installed."
    PAUSE
    EXIT /B
)

REM --- Get user input ---
SET /P Head=Enter Source Branch (head): 
SET /P Base=Enter Target Branch (base): 
SET /P Title=Enter Pull Request Title: 
SET /P NewBranch=Enter optional new branch name (leave empty to skip): 

REM FOR /F "usebackq delims=" %%b IN (`git rev-parse --abbrev-ref HEAD`) DO SET CurrentBranch=%%b

REM FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO ( SET "CurrentBranch=%%b" )

REM ECHO Current branch: [%CurrentBranch%]
REM ECHO Head branch:    [%Head%]

REM PAUSE
REM EXIT /B


REM --- Optional body ---
SET Body=Auto-generated PR from script

REM --- Push current branch ---
git push origin %Head%

REM --- Create Pull Request ---
ECHO Creating Pull Request...
gh pr create --title "%Title%" --body "%Body%" --base "%Base%" --head "%Head%"
IF ERRORLEVEL 1 (
    ECHO "Failed to create Pull Request."
    PAUSE
    EXIT /B
)

REM --- Get PR number using gh CLI JSON parsing ---
FOR /F "delims=" %%I IN ('gh pr list --state open --head "%Head%" --json number --jq ".[0].number"') DO SET PR_Number=%%I

IF NOT DEFINED PR_Number (
    ECHO "Error: Unable to find Pull Request number."
    PAUSE
    EXIT /B
)

ECHO Pull Request created with number #%PR_Number%

FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO (
	SET "CurrentBranch=%%b"
)
ECHO Current branch #1: [%CurrentBranch%]

REM --- Merge Pull Request ---
ECHO Merging Pull Request...
gh pr merge %PR_Number% --merge --subject "Merge PR #%PR_Number% %Title%"
IF ERRORLEVEL 1 (
    ECHO "Failed to merge Pull Request."
    PAUSE
    EXIT /B
)

ECHO Pull Request #%PR_Number% successfully merged.


FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO (
	SET "CurrentBranch=%%b"
)
ECHO Current branch #2: [%CurrentBranch%]

REM --- Get repo owner/name path ---
FOR /F "tokens=*" %%I IN ('gh repo view --json nameWithOwner --jq ".nameWithOwner"') DO SET RepoPath=%%I

REM --- Delete source branch via GitHub API ---
ECHO Deleting source branch %Head%...
gh api repos/%RepoPath%/git/refs/heads/%Head% -X DELETE
IF ERRORLEVEL 1 (
    ECHO "Failed to delete branch %Head%."
    PAUSE
    EXIT /B
)

ECHO Source branch %Head% successfully deleted.


FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO (
	SET "CurrentBranch=%%b"
)
ECHO Current branch #3: [%CurrentBranch%]


REM --- Prune deleted branches locally ---
ECHO Cleaning up local references...
git fetch origin --prune
IF ERRORLEVEL 1 (
    ECHO "Failed to prune deleted branches."
    PAUSE
    EXIT /B
)

ECHO Local references successfully pruned.


FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO (
	SET "CurrentBranch=%%b"
)
ECHO Current branch #4: [%CurrentBranch%]


IF NOT "%NewBranch%"=="" (

    REM Determine current local branch
	FOR /F "delims=" %%b IN ('git rev-parse --abbrev-ref HEAD') DO (
		SET "CurrentBranch=%%b"
	)

	ECHO Current branch #5: [%CurrentBranch%]
	ECHO Head branch:    [%Head%]
)


ECHO Done.
PAUSE
