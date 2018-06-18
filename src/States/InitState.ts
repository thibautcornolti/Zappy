import * as THREE from "three";
import IState from "./IState";
import StateShare from "./StateShare";
import SocketCom from "../SocketCom";
import StateMachine from "../StateMachine";
import CoreState from "./CoreState";
import GUIManagger from "../GUIManager";
import LoadingBar from "../LoadingBar";

export default class InitState implements IState {
    private share: StateShare;
    private loadCore: boolean;
    private isMapLoaded: boolean;
    private isSocketConnected: boolean;
    private loading: LoadingBar;
    // private progressB : HTMLElementTagNameMap["div"];
    // private progressW : HTMLElementTagNameMap["div"];

    constructor(share: StateShare) {
        this.loadCore = false;
        this.isMapLoaded = false;
        this.isSocketConnected = false;
        this.share = share;
        this.loading = new LoadingBar();

        // this.progressW = document.createElement('div');
        // this.progressB = document.createElement('div');
        // this.progressB.classList.add("progress-bar");
        // this.progressW.classList.add("progress-wrap");
        // this.progressW.appendChild(this.progressB);
    }

    private loadMusic() {
        let sound = new THREE.Audio(GUIManagger.getInstance().getAudio());
        let audioLoader = new THREE.AudioLoader();
        audioLoader.load('sounds/music.ogg', function(buffer: THREE.AudioBuffer) {
            sound.setBuffer(buffer);
            sound.setLoop( true );
            sound.setVolume( 0.5 );
            sound.play();
        }, () => {}, () => {});
    }

    private initSocket() {
        this.share.addKey("socket", new SocketCom(33333, "127.0.0.1", () => {
            this.isSocketConnected = true;
        }, () => {
            this.loading.setError("Impossible de se connecter au serveur Zappy");
            // alert("Impossible de se connecter au serveur Zappy");
        }));
    }

    public init() {
        this.loading.show();
        this.isSocketConnected = true;
        // this.initSocket();
        this.share.getAssetsPool().loadAssets("map", "models/map/map.obj", "models/map/map.mtl", (evt) => {
            this.loading.setPercentage((evt.loaded / evt.total) * 100);
        }, () => {
            this.isMapLoaded = true;
        }, () => {
            alert("Impossible de charger la map");
        });
        // this.loadMusic();
    }

    public update() {
        if (this.isMapLoaded && this.isSocketConnected && !this.loadCore) {
            console.log("Loaded");
            this.loading.hide();
            this.loadCore = true;
            (<StateMachine> this.share.getKey("stateMachine")).push(new CoreState(this.share));
        }
    }

    public getName() {
        return "init";
    }
};