import IState from "./IState"
import GUIManager from "../GUIManager"
import StateShare from "./StateShare";
import MainScene from "../Scenes/MainScene";
import {Vector3} from "three";

export default class CoreState implements IState {
    private manager: GUIManager;
    private state: StateShare;
    private mainMap: MainScene;
    private camPos: Vector3;

    private inc: number;

    public constructor(state: StateShare) {
        this.inc = 0;
        this.mainMap = new MainScene(state);
        this.manager = GUIManager.getInstance();
        this.state = state;
        this.camPos = new Vector3();
    }

    public init() {
        this.mainMap.generate();
        this.camPos = Object.assign({}, this.manager.getCamera().position);
        this.state.getSocket().setOnData(this.onSocketData);
        console.log("core");
    }

    private onSocketData(resp: string) {
        console.log(resp);
    }

    public update() {
        let scale = (2 * Math.PI) / 2000;
        this.inc += 1;
        if (this.inc == 2000)
            this.inc = 0;
        let camPos = Object.assign({}, this.camPos);
        camPos.y += Math.sin(scale * this.inc) * 2;
        this.manager.getCamera().position.set(camPos.x, camPos.y, camPos.z);
    }

    public getName() {
        return "core";
    }
}