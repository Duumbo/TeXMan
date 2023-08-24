# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: Dimitri Bonanni-Surprenant <bond2102@usherbrooke.ca>
pkgname=texman
pkgver=0.1.0
pkgrel=1
pkgdesc="A LaTeX template manager."
epoch=
arch=(x86_64)
url="https://github.com/Duumbo/TeXMan"
license=('MIT')
groups=()
depends=()
makedepends=("rust")
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.gz")
noextract=()
sha256sums=('712dbda844fc8cbb7c35d2c5393acdfb084f5b6a67b7d88b9e9a091ff1944a38')
validpgpkeys=()

#prepare() {
#}

build() {
	cd "$pkgname-$pkgver"
	make
}

check() {
	cd "$pkgname-$pkgver"
	make check
}

package() {
	cd "$pkgname-$pkgver"
    mkdir -p "$pkgdir/usr/bin"
	make DESTDIR="$pkgdir/" SOURCE="$srcdir/$pkgname-$pkgver/" install
}
