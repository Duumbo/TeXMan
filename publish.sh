set -xe
mkdir build/texman-0.1.0
cd build
cp -r ../src texman-0.1.0/src
cp ../Makefile texman-0.1.0/
cp ../Cargo.toml texman-0.1.0/
tar -czvf texman-0.1.0.tar.gz texman-0.1.0
cp ../PKGBUILD PKGBUILD
updpkgsums
cd ..
