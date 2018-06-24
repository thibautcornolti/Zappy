import * as THREE from "three";
import IState from "./IState"
import GUIManager from "../GUIManager"
import StateShare from "./StateShare";
import MainScene from "../Scenes/MainScene";
import {Vector3} from "three";
import {Raycaster} from "three";
import {Vector2} from "three";
import {Object3D} from "three";
import {ITileResp} from "../ICom";
import TileInfo from "../Entity/TileInfo";

export default class CoreState implements IState {
    private manager: GUIManager;
    private state: StateShare;
    private mainMap: MainScene;
    private tileInfo: TileInfo;
    private camPos: Vector3;

    private eventBinding: { [index: string]: (data: Object) => void };
    private inc: number;

    public constructor(state: StateShare) {
        this.mainMap = new MainScene(state);
        this.manager = GUIManager.getInstance();
        this.state = state;
        this.camPos = new Vector3();

        this.inc = 0;
        this.tileInfo = new TileInfo();

        document.addEventListener('mousedown', this.onClick, false);

        this.eventBinding = {};
        this.eventBinding["player-join"] = this.mainMap.playerJoin.bind(this.mainMap);
        this.eventBinding["player-death"] = this.mainMap.playerDeath.bind(this.mainMap);
        this.eventBinding["player-move"] = this.mainMap.playerMove.bind(this.mainMap);
        this.eventBinding["player-turn"] = this.mainMap.playerTurn.bind(this.mainMap);
        this.eventBinding["player-look"] = this.mainMap.playerLook.bind(this.mainMap);
        this.eventBinding["player-inventory"] = this.mainMap.playerInventory.bind(this.mainMap);
        this.eventBinding["item-pickup"] = this.mainMap.itemPickup.bind(this.mainMap);
        this.eventBinding["item-drop"] = this.mainMap.itemDrop.bind(this.mainMap);
        this.eventBinding["incantation-start"] = this.mainMap.playerIncantationStart.bind(this.mainMap);
        this.eventBinding["incantation-fail"] = this.mainMap.playerIncantationFail.bind(this.mainMap);
        this.eventBinding["incantation-success"] = this.mainMap.playerIncantationSuccess.bind(this.mainMap);
        this.eventBinding["egg-drop"] = this.mainMap.playerDropEgg.bind(this.mainMap);
        this.eventBinding["egg-hatch"] = this.mainMap.playerHatchEgg.bind(this.mainMap);
        this.eventBinding["broadcast"] = this.mainMap.playerBroadcast.bind(this.mainMap);
    }

    public init() {
        this.mainMap.generate();
        this.camPos = Object.assign({}, this.manager.getCamera().position);
        this.state.getSocket().setOnData(this.onSocketData);
        this.state.getSocket().sendJSON({"command": "entities"});
    }

    private setInfoTile(data: ITileResp) {
        data.data.forEach((elem) => {
            if (elem.type === "player") {
                (elem as any).ids.forEach((e: number) => {
                    this.state.getSocket().sendJSON({"command": "player", id: e})
                });
            }
        });
        this.tileInfo.setInfo(data);
        this.tileInfo.show();
    }

    private refreshOverlay(json: any) {
        let event = [
            "player-join",
            "player-move",
            "player-death",
            "item-drop",
            "item-pickup",
            "incantation-success",
        ];
        if (this.tileInfo.isOpen() && event.indexOf(json["event-type"]) !== -1) {
            let pos = this.tileInfo.getPosition();
            this.state.getSocket().sendJSON({"command": "tile", pos: {x: pos.x, y: pos.y}});
        }
    }

    private treatData(json: any) {
        if (json.type === "response") {
            if (json['response-type'] === "tile")
                this.setInfoTile(json);
            else if (json['response-type'] === "player")
                this.tileInfo.addPlayerInfo(json.data);
            else if (json['response-type'] === "entities")
                this.mainMap.initMapTile(json);
        } else if (json.type === "event") {
            if (this.eventBinding[json["event-type"]])
                this.eventBinding[json["event-type"]](json.data);
            this.refreshOverlay(json);
        }
    }

    private onSocketData = (data: string) => {
        let resp = data.split('\n');
        console.log(data)

        resp.forEach((elem) => {
            let json;

            if (!elem.length)
                return;
            try {
                json = JSON.parse(elem);
            } catch (e) {
                console.warn("Unable to parse a response: ", elem, e);
                return;
            }
            this.treatData(json);
        });
    };

    public async update() {
        this.mainMap.update();


        //TODO move this in a class
        //TODO change inc by time
        let scale = (2 * Math.PI) / 2000;
        this.inc += 1;
        if (this.inc == 2000)
            this.inc = 0;
        let camPos = Object.assign({}, this.camPos);
        camPos.y += Math.sin(scale * this.inc) * 2;
        this.manager.getCamera().lookAt(10, 0, 15);
        this.manager.getCamera().position.set(camPos.x, camPos.y, camPos.z);
    }

    public getName() {
        return "core";
    }

    private askTileInfo(pos: Vector2) {
        if (pos.x >= 0 && pos.x < this.state.getMapSize().x && pos.y >= 0 && pos.y < this.state.getMapSize().y)
            this.state.getSocket().sendJSON({"command": "tile", pos: {x: pos.x, y: pos.y}});
        else
            this.tileInfo.hide();
    }

    private onClick = (evt: MouseEvent) => {
        evt.preventDefault();
        let raycaster = new Raycaster();
        let mouse = new Vector2();
        mouse.x = (evt.clientX / GUIManager.getInstance().getRenderer().domElement.clientWidth) * 2 - 1;
        mouse.y = -(evt.clientY / GUIManager.getInstance().getRenderer().domElement.clientHeight) * 2 + 1;
        raycaster.setFromCamera(mouse, GUIManager.getInstance().getCamera());
        let obj: Object3D[] = [];
        GUIManager.getInstance().getScene().traverse((elem) => {
            if (elem.type === "Mesh")
                obj.push(elem);
        });
        let ray = raycaster.intersectObjects(obj);
        for (let i = 0; i < ray.length; i++) {
            let elem = ray[i];
            if (elem.object.name === "Grass_Block") {
                this.askTileInfo(this.mainMap.reversePosition(elem.point));
                return;
            }
        }
        this.tileInfo.hide();
    };
}