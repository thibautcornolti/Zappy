import * as THREE from "three";
import {Vector2, Vector3} from "three";
import AssetsPool from "../AssetsPool";
import {GLTF} from "three-gltf-loader";
import GUIManagger from "../GUIManager";
import {IDataResp, IEntitiesResp, ITileResp} from "../ICom";
import Dropable from "./Dropable";
import Player from "./Player";

interface IType {
    name: string,
    id: number
}

interface IEntitiesContent {
    type: string
    data: [{
        pos: {
            x: number,
            y: number,
        }
    }]
}

interface IPlayerEntity {
    pos: {
        x: number,
        y: number
    },
    id: number,
    team: string,
    facing: "N" | "E" | "S" | "O",
    level: number
}

let facingTable: {[index: string]: number} = {
    N: Math.PI / 2,
    E: 0,
    S: -Math.PI / 2,
    O: Math.PI
};

let typeTable: {[index: string]: IType} = {
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
    },
    player: {
        name: "chicken",
        id: 7
    }
};

export default class MapEntity {
    private map: GLTF;
    private posStart: Vector2;
    private posEnd: Vector2;
    private assetPool: AssetsPool;
    private mapSize: Vector2;

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
    }

    private calcPosEntity(id: number, pos: Vector2) : Vector2 {
        let ret = new Vector2();
        let mapRatio = new Vector2((this.posEnd.x - this.posStart.x) / this.mapSize.x,
            (this.posEnd.y - this.posStart.y) / this.mapSize.y);
        let tileStart = new Vector2(mapRatio.x * pos.x, mapRatio.y * pos.y);
        let tileEnd = new Vector2(mapRatio.x * (pos.x + 1), mapRatio.y * (pos.y + 1));
        let posInTile = new Vector2(id % 3, Math.floor(id / 3));
        let tileScale = new Vector2((tileEnd.x - tileStart.x) / 3, (tileEnd.y - tileStart.y) / 3);

        ret.x = this.posStart.x + tileStart.x + posInTile.x * tileScale.x;
        ret.y = this.posStart.y + tileStart.y + posInTile.y * tileScale.y;
        return ret;
    }

    private initPlayerEntity(pos: Vector2, facing: string) {
        let mapRatio = new Vector2((this.posEnd.x - this.posStart.x) / this.mapSize.x,
            (this.posEnd.y - this.posStart.y) / this.mapSize.y);
        let posStart = new Vector2(this.posStart.x + pos.x * mapRatio.x, this.posStart.y + pos.y * mapRatio.y);
        let posEnd = new Vector2(this.posStart.x + (pos.x + 1) * mapRatio.x, this.posStart.y + (pos.y + 1) * mapRatio.y);
        let player = new Player(this.assetPool, new Vector2(posStart.x + (posEnd.x - posStart.x) / 2, posStart.y + (posEnd.y - posStart.y) / 2));
        player.setRotation(new Vector3(0, facingTable[facing], 0));
    }

    public initEntitiesTile(resp: IEntitiesContent) {
        let type = (typeTable[resp.type] || {name: resp.type, id: -1});
        resp.data.forEach((elem) => {
            if (type.id >= 0 && type.id < 7) {
                let drop;
                try {
                    drop = new Dropable(this.assetPool, type.name);
                } catch (e) {
                    console.warn("Unable to create a \"" + ((type) ? type.name : "Undefined") + "\" Dropable");
                    return;
                }
                let pos = this.calcPosEntity(type.id, new Vector2(elem.pos.x, elem.pos.y));
                drop.setPosition(new Vector3(pos.x, 0, pos.y));
            } else if (type.id === 7) {
                let info = (elem as IPlayerEntity);
                this.initPlayerEntity(new Vector2(info.pos.x, info.pos.y), info.facing);
            }
        });
    }
}