# Contributor: George Kibardin <george dot kibardin at gmail dot com>
pkgname=xkb-layout-notifier
pkgver=0.1
pkgrel=2
pkgdesc="Executes arbitrary shell command when current xkb layout changes"
arch=('i686' 'x86_64')
url="http://code.google.com/p/xkb-layout-notifier"
license=('BSD')
depends=('libxext' 'libx11')
source=(http://xkb-layout-notifier.googlecode.com/files/$pkgname-$pkgver.tar.gz)
md5sums=('c5bc4c427c8a43f29bb20caacdf9f94f')

build() {
  cd "$srcdir"
  make || return 1
}

package() {
  install -D -m 755 xkb-layout-notifier $pkgdir/usr/bin/xkb-layout-notifier
  install -D -m 644 xkb-layout-notifier.1 $pkgdir/usr/share/man/man1/xkb-layout-notifier.1
}
