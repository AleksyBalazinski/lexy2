./build/lexy2 test.l2
if ($LASTEXITCODE -ne 0) {
    break
}
clang test.ll
./a.exe