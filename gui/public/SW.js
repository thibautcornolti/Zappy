(function (self) {
    const version = "v1.0";

    self.addEventListener("install", (ev) => {
        ev.waitUntil(
            caches.open(version).then((cache) =>
                cache.addAll([
                    "https://s3.eu-west-3.amazonaws.com/thibaut-public-asset/map_zappy.bin"
                ])
            )
        );
    });

    self.addEventListener('fetch', (ev) => {
        ev.respondWith(caches.match(ev.request).then((res) => {
            if (res) return res;
            return fetch(ev.request);
        }));
    });

    self.addEventListener('activate', (ev) => {
        let cacheWhitelist = [version];

        ev.waitUntil(
            caches.keys().then((keyList) => {
                return Promise.all([keyList.map((key) => {
                    if (cacheWhitelist.indexOf(key) === -1) {
                        return caches.delete(key);
                    }
                    return undefined;
                })]);
            })
        );
    });
})(self);