import * as THREE from "three";
import {Vector2, Vector3, Object3D} from "three";
import AssetsPool from "../AssetsPool";
import {GLTF} from "three-gltf-loader";
import GUIManagger from "../GUIManager";
import {IDataResp, IEntitiesResp, IPlayerEntity, IItemEntity, ITileResp} from "../ICom";
import Dropable from "./Dropable";
import Player from "./Player";

interface IType {
    name: string,
    id: number
}

interface IEntitiesContent {
    type: string,
    data: [{
        pos: {
            x: number,
            y: number,
        },
        amount?: number,
    }],
}

let facingTable: {[index: string]: number} = {
    N: Math.PI / 2,
    E: 0,
    S: -Math.PI / 2,
    W: Math.PI
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

interface ITile {
    name: string,
    amount: number;
    dropable: Dropable,
}

export default class MapEntity {
    private map: GLTF;
    private mapItems: {[key: string]: ITile[]};
    private posStart: Vector2;
    private posEnd: Vector2;
    private assetPool: AssetsPool;
    private mapSize: Vector2;

    private player: {[index: number]: {info: IPlayerEntity, obj: Player}};

    private content: Map<{ x: number, y: number }, Object>;

    constructor(assetsPool: AssetsPool, mapSize: Vector2) {
        this.posStart = new Vector2(-6, -3);
        this.posEnd = new Vector2(28, 27);

        this.content = new Map();
        this.player = {};

        this.mapItems = {};
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

    private initPlayerEntity(pos: Vector2, facing: string): Player {
        let mapRatio = new Vector2((this.posEnd.x - this.posStart.x) / this.mapSize.x,
            (this.posEnd.y - this.posStart.y) / this.mapSize.y);
        let posStart = new Vector2(this.posStart.x + pos.x * mapRatio.x, this.posStart.y + pos.y * mapRatio.y);
        let posEnd = new Vector2(this.posStart.x + (pos.x + 1) * mapRatio.x, this.posStart.y + (pos.y + 1) * mapRatio.y);

        let player = new Player(this.assetPool, new Vector2(posStart.x + (posEnd.x - posStart.x) / 2, posStart.y + (posEnd.y - posStart.y) / 2));
        player.setRotation(new Vector3(0, facingTable[facing], 0));
        return player;
    }

    public getDropable(data: IItemEntity): Dropable | undefined {
        let drop;
        let type = (typeTable[data.item] || {name: data.item, id: -1});
        try {
            drop = new Dropable(this.assetPool, type.name);
        } catch (e) {
            console.warn("Unable to create a \"" + ((type) ? type.name : "Undefined") + "\" Dropable");
            return;
        }
        let pos = this.calcPosEntity(type.id, new Vector2(data.pos.x, data.pos.y));
        drop.setPosition(new Vector3(pos.x, 0, pos.y));
        return drop;
    }

    public initEntitiesTile(resp: IEntitiesContent) {
        let type = (typeTable[resp.type] || {name: resp.type, id: -1});
        resp.data.forEach((elem) => {
            if (type.id >= 0 && type.id < 7) {
                let drop = this.getDropable({
                    id: -1,
                    item: resp.type,
                    pos: elem.pos,
                });
                if (!drop)
                    return ;
                const entry = {
                    name: resp.type,
                    amount: elem.amount as number,
                    dropable: drop,
                };
                const key = new String(elem.pos.x) + ',' + new String(elem.pos.y);
                const it = this.mapItems[key];
                if (it)
                    it.push(entry);
                else
                    this.mapItems[key] = [entry];
            } else if (type.id === 7) {
                let info = (elem as IPlayerEntity);
                let player = this.initPlayerEntity(new Vector2(info.pos.x, info.pos.y), info.facing);
                this.player[info.id] = {info: info, obj: player};
            }
        });
    }

    private convertPosition(pos: Vector2) : Vector3 {
        let ret = new Vector3();
        let mapRatio = new Vector2((this.posEnd.x - this.posStart.x) / this.mapSize.x,
            (this.posEnd.y - this.posStart.y) / this.mapSize.y);
        let tileStart = new Vector2(mapRatio.x * pos.x, mapRatio.y * pos.y);
        let tileEnd = new Vector2(mapRatio.x * (pos.x + 1), mapRatio.y * (pos.y + 1));
        let posInTile = new Vector2((tileEnd.x - tileStart.x) / 2, (tileEnd.y - tileStart.y) / 2);

        ret.x = this.posStart.x + tileStart.x + posInTile.x;
        ret.y = 0;
        ret.z = this.posStart.y + tileStart.y + posInTile.y;
        return ret;
    }


    //EVENT
    public playerJoin(data: IPlayerEntity) {
        let player = this.initPlayerEntity(new Vector2(data.pos.x, data.pos.y), data.facing);
        this.player[data.id] = {info: data, obj: player};
    }

    public playerDeath(data: IPlayerEntity) {
        if (this.player[data.id]) {
            this.player[data.id].obj.remove();
            delete this.player[data.id];
        }
    }

    public playerMove(data: IPlayerEntity) {
        if (this.player[data.id]) {
            this.player[data.id].info.pos = data.pos;
            this.player[data.id].obj.setPosition(this.convertPosition(new Vector2(data.pos.x, data.pos.y)));
        }
    }

    public playerTurn(data: IPlayerEntity) {
        if (this.player[data.id]) {
            this.player[data.id].info.facing = data.facing;
            this.player[data.id].obj.setRotation(new Vector3(0, facingTable[data.facing], 0));
        }
    }

    public itemPickup(data: IItemEntity) {
        const key = new String(data.pos.x) + ',' + new String(data.pos.y);
        const items = this.mapItems[key];
        if (items == undefined)
            return ;
        for (let i = items.length - 1; i >= 0; i--) {
            if (items[i].name == data.item) {
                items[i].amount -= 1;
                if (items[i].amount == 0) {
                    items[i].dropable.remove()
                    items.splice(i, 1);
                }
                break;
            }
        }
    }

    public itemDrop(data: IItemEntity) {
        const key = new String(data.pos.x) + ',' + new String(data.pos.y);
        const items = this.mapItems[key];
        
        let getItemEntry = (): ITile | undefined => {
            let drop;
            drop = this.getDropable(data)
            if (drop)
                return {
                    name: data.item,
                    amount: 1,
                    dropable: drop,
                };
        }

        if (items == undefined) {
            const entry = getItemEntry();
            if (!entry)
                return ;
            this.mapItems[key] = [entry]
            
        } else {
            for (let i = items.length - 1; i >= 0; i--) {
                if (items[i].name == data.item) {
                    items[i].amount += 1;
                    console.log("new amount: " + items[i].amount)
                    return ;
                }
            }
            const entry = getItemEntry();
            if (!entry)
                return ;
            items.push(entry);
        }
    }
}