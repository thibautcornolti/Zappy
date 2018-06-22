import * as THREE from "three";
import {Vector2, Vector3} from "three";
import AssetsPool from "../AssetsPool";
import {GLTF} from "three-gltf-loader";
import GUIManagger from "../GUIManager";
import {IDataResp, ITileResp} from "../ICom";
import Dropable from "./Dropable";

interface IType {
    name: string,
    id: number
}

let typeTable: {[index:string]: IType} = {
    linemate: {
        name: "coal",
        id: 0
    },
    deraumere: {
        name: "iron",
        id: 1
    },
    sibur: {
        name: "lapis",
        id: 2
    },
    mendiane: {
        name: "gold",
        id: 3
    },
    phiras: {
        name: "emerald",
        id: 4
    },
    thystame: {
        name: "diamond",
        id: 5
    },
    food: {
        name: "food",
        id: 6
    }
};

export default class MapEntity {
    private map: GLTF;
    private posStart: Vector2;
    private posEnd: Vector2;
    private assetPool: AssetsPool;
    private mapSize: Vector2;

    // private content: Map<Vector2, Array<IDataResp>>;
    private content: Map<{ x: number, y: number }, Object>;

    constructor(assetsPool: AssetsPool, mapSize: Vector2) {
        this.posStart = new Vector2(-6, -3);
        this.posEnd = new Vector2(28, 27);

        this.content = new Map();

        this.mapSize = mapSize;
        this.assetPool = assetsPool;
        this.map = assetsPool.getGltfAssets("map");
        this.map.scene.position.set(0, -3.6, 0);
        this.map.scene.rotation.y = (Math.PI / 2) * 3;
        GUIManagger.getInstance().getScene().add(this.map.scene);
    }

    public getPosStart() {
        return this.posStart;
    }

    public getPosEnd() {
        return this.posEnd;
    }

    public update() {
    }

    private setPosDragable(drop: Dropable, posStart: Vector2, posEnd: Vector2, pos: Vector2) {
        let ratio = new Vector2(posEnd.x - posStart.x, posEnd.y - posStart.y);
        let scale = new Vector2(ratio.x / 3, ratio.y / 3);

        drop.setPosition(new Vector3(posStart.x + scale.x * pos.x, 0, posStart.y + scale.y * pos.y));
    }

    public setTile(data: ITileResp, posStart: Vector2, posEnd: Vector2) {
        let obj = ([] as any);
        let ressources = data.data;
        ressources.forEach((elem) => {
            let type: IType | undefined;
            let drop;
            try {
                type = (typeTable[(elem.type as string)] || {name: elem.type, id: -1});
                drop = new Dropable(this.assetPool, type.name);
            } catch (e) {
                console.warn("Unable to create a \"" + ((type) ? type.name : "Undefined") + "\" Dropable");
                return;
            }
            if (type.id >= 0 && type.id <= 6)
                this.setPosDragable(drop, posStart, posEnd, new Vector2(type.id % 3, Math.floor(type.id / 3)));
                // console.log(type.id, new Vector2(type.id % 3, Math.floor(type.id / 3)));
                // drop.setPosition(new )
                // else
            // drop.setPosition(new Vector3(posStart.x, 0, posStart.y));
            obj.push(drop);
        });
        this.content.set({x: data.pos.x, y: data.pos.y}, {data: data.data, obj: obj});

        // let geometry = new THREE.BoxGeometry(1, 1, 1);
        // let material = new THREE.MeshBasicMaterial({color: 0x00ff00});
        // let cube = new THREE.Mesh(geometry, material);

        // cube.position.set(posStart.x, 0, posStart.y);

        // GUIManagger.getInstance().getScene().add(cube);
    }
}