import IState from "./IState";
import StateShare from "./StateShare";
import SocketCom from "../SocketCom";
import StateMachine from "../StateMachine";
import CoreState from "./CoreState";

export default class InitState implements IState {
    private share: StateShare;
    private loadCore: boolean;
    private isMapLoaded: boolean;
    private isSocketConnected: boolean;

    constructor(share: StateShare) {
        this.loadCore = false;
        this.isMapLoaded = false;
        this.isSocketConnected = false;
        this.share = share;
    }

    public init() {
        this.share.addKey("socket", new SocketCom(33333, "127.0.0.1", () => {
            this.isSocketConnected = true;
        }, () => {
            // alert("Impossible de se connecter au serveur Zappy");
        }));
        this.share.getAssetsPool().loadAssets("map", "models/map/map.obj", "models/map/map.mtl", undefined, () => {
            this.isMapLoaded = true;
        }, () => {
            // alert("Impossible de charger la map");
        });
    }

    public update() {
        if (this.isMapLoaded && this.isSocketConnected && !this.loadCore) {
            console.log("Loaded");
            this.loadCore = true;
            (<StateMachine> this.share.getKey("stateMachine")).push(new CoreState(this.share));
        }
    }
};