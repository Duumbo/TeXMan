set -xe
mkdir build/texman-0.1.1
cd build
cp -r ../src texman-0.1.1/src
cp ../Makefile texman-0.1.1/
cp ../Cargo.toml texman-0.1.1/
tar -czvf texman-0.1.1.tar.gz texman-0.1.1
cp ../PKGBUILD PKGBUILD
updpkgsums
cd ..
