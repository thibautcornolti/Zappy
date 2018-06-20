import * as THREE from "three"
import * as path from "path"
import GLTFLoader, {GLTF, GLTFLoaderlol} from "three-gltf-loader";
// import {Group} from "three"
import "obj-loader";
import "mtl-loader";
import "gltf-loader";
import "draco-loader";
import {CubeTexture, MaterialCreator, Vector3} from "three";


interface IAssets {
    [key: string]: any;
}

export default class AssetsPool {
    private assets: IAssets;

    constructor() {
        this.assets = {};
    }

    public loadGlTFProm(key: string, pathGlTF: string, onLoad?: (obj: GLTF) => void, onProgress?: (evt: ProgressEvent) => void, onError?: (err: ErrorEvent) => void) {
        return new Promise((resolve, reject) => {
            this.loadGlTF(key, pathGlTF, (obj) => {
                if (onLoad)
                    onLoad(obj);
                resolve();
            }, onProgress, (err) => {
                if (onError)
                    onError(err);
                reject();
            });
        })
    }

    public loadGlTF(key: string, pathGlTF: string, onLoad?: (obj: GLTF) => void, onProgress?: (evt: ProgressEvent) => void, onError?: (err: ErrorEvent) => void) {
        let gltfloader = (new (THREE as any).GLTFLoader() as GLTFLoaderlol);
        gltfloader.load(pathGlTF, (gltf) => {
            if (onLoad)
                onLoad(gltf);
            this.assets[key] = gltf;
        }, onProgress, onError);
    }

    public loadCubeTextureProm(key: string, urls: Array<string>, onLoad?: (obj: CubeTexture) => void, onProgress?: (evt: ProgressEvent) => void, onError?: (err: ErrorEvent) => void) {
        return new Promise((resolve, reject) => {
            this.loadCubeTexture(key, urls, (obj) => {
                if (onLoad)
                    onLoad(obj);
                resolve();
            }, onProgress, (err) => {
                if (onError)
                    onError(err);
                reject();
            });
        })
    }

    public loadCubeTexture(key: string, urls: Array<string>, onLoad?: (obj: CubeTexture) => void, onProgress?: (evt: ProgressEvent) => void, onError?: (err: ErrorEvent) => void) {
        let cube = new THREE.CubeTextureLoader();
        cube.load(urls, (text) => {
            if (onLoad)
                onLoad(text);
            this.assets[key] = text;
        }, onProgress, onError);
    }

    public loadAssets(key: string, pathObj: string, pathMat: string, onProgress?: (evt: ProgressEvent) => void, onFinish?: (obj: THREE.Group) => void, onError?: (evt: ErrorEvent) => void) {
        let mtlload = new THREE.MTLLoader();
        mtlload.setPath(path.dirname(pathMat) + '/');
        mtlload.load(path.basename(pathMat), (materials) => {
            materials.preload();

            let objload = new THREE.OBJLoader();
            objload.setMaterials(materials);
            objload.setPath(path.dirname(pathObj) + '/');
            objload.load(path.basename(pathObj), (object) => {
                this.assets[key] = object;
                if (onFinish)
                    onFinish(object);
            }, onProgress, onError);

        });
    }

    public getAssets(key: string): THREE.Object3D {
        return this.assets[key];
    }

    public getGltfAssets(key: string): GLTF {
        return this.assets[key];
    }

    public getCubeTexture(key: string): CubeTexture {
        return this.assets[key];
    }

    public unloadAssets(key: string) {
        delete this.assets[key];
    }
}