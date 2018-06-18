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
    private progressB : HTMLElementTagNameMap["div"];
    private progressW : HTMLElementTagNameMap["div"];

    constructor(share: StateShare) {
        this.loadCore = false;
        this.isMapLoaded = false;
        this.isSocketConnected = false;
        this.share = share;

        this.progressW = document.createElement('div');
        this.progressB = document.createElement('div');
        this.progressB.classList.add("progress-bar");
        this.progressW.classList.add("progress-wrap");
        this.progressW.appendChild(this.progressB);
    }

    public init() {
        document.body.appendChild(this.progressW);

        // this.isSocketConnected = true;
        this.share.addKey("socket", new SocketCom(33333, "127.0.0.1", () => {
            this.isSocketConnected = true;
        }, () => {
            this.progressB.style.backgroundColor = "tomato";
            this.progressW.style.backgroundColor = "tomato";
            this.progressW.textContent = "Impossible de se connecter au serveur Zappy";
            // alert("Impossible de se connecter au serveur Zappy");
        }));
        this.share.getAssetsPool().loadAssets("map", "models/map/map.obj", "models/map/map.mtl", (evt) => {
            this.progressB.style.width = (evt.loaded / evt.total) * 100 + "%";
        }, () => {
            this.isMapLoaded = true;
        }, () => {
            // alert("Impossible de charger la map");
        });
    }

    public update() {
        if (this.isMapLoaded && this.isSocketConnected && !this.loadCore) {
            console.log("Loaded");
            this.progressW.remove();
            this.loadCore = true;
            (<StateMachine> this.share.getKey("stateMachine")).push(new CoreState(this.share));
        }
    }

    public getName() {
        return "init";
    }
};