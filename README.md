# Website project

1. Install vcpkg:
```cmd
git clone https://github.com/Microsoft/vcpkg.git
```
if WINDOWS:
```cmd
.\vcpkg\bootstrap-vcpkg.bat
```
else:
```shell
./vcpkg/bootstrap-vcpkg.sh
```

```cmd
./vcpkg/vcpkg integrate install

./vcpkg/vcpkg list

```
2. Install all packages from `requirements.txt`
