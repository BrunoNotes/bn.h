$DirectoryToCreate = "bin"
if (-not (Test-Path -LiteralPath $DirectoryToCreate -PathType Container)) {
    New-Item -Path $DirectoryToCreate -ItemType Directory
}
clang .\test\main.c -DDEBUG -g -o .\bin\main.exe
& ".\bin\main.exe"

