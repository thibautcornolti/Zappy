import * as THREE from "three"
import * as path from "path"
// import {Group} from "three"
import {MTLLoader, OBJLoader} from 'three-obj-mtl-loader'
import {MaterialCreator} from "three";


interface IAssets {
    [key: string] : THREE.Group;
}

export default class AssetsPool {
    private assets: IAssets;

    constructor() {
        this.assets = {};
        // THREE.Loader.Handlers.add(/\.dds$/i, new THREE.DDSLoader());
    }

    loadAssets(key: string, pathObj: string, pathMat: string, onProgress?: (evt: ProgressEvent) => void, onFinish?: (obj: THREE.Group) => void, onError?: (evt: ErrorEvent) => void) {
        console.log("start loading");
        let mtlload = new MTLLoader();
        mtlload.setPath(path.dirname(pathMat) + '/');
        mtlload.load(path.basename(pathMat), (materials) => {
            materials.preload();

            let objload = new OBJLoader();
            objload.setMaterials(materials);
            objload.setPath(path.dirname(pathObj) + '/');
            objload.load(path.basename(pathObj), (object) => {
                this.assets[key] = object;
                if (onFinish) {
                    onFinish(object);
                }
                console.log("Loaded:", pathObj);
            }, onProgress, onError);

        });
    }

    getAssets(key: string) : THREE.Object3D {
        return this.assets[key];
    }

    unloadAssets(key: string) {
        delete this.assets[key];
    }
}