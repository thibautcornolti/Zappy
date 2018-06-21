import * as THREE from "three";
import IState from "./IState";
import StateShare from "./StateShare";
import SocketCom from "../SocketCom";
import StateMachine from "../StateMachine";
import CoreState from "./CoreState";
import GUIManagger from "../GUIManager";
import LoadingBar from "../LoadingBar";
import {IMapSize} from "../ICom";

export default class InitState implements IState {
    private share: StateShare;
    private loadCore: boolean;
    private isMapLoaded: boolean;
    private isSocketConnected: boolean;
    private loading: LoadingBar;
    private socket: SocketCom | null;

    constructor(share: StateShare) {
        this.loadCore = false;
        this.isMapLoaded = false;
        this.isSocketConnected = false;
        this.share = share;
        this.loading = new LoadingBar();
        this.socket = null;
    }

    private loadMusic() {
        let sound = new THREE.Audio(GUIManagger.getInstance().getAudio());
        let audioLoader = new THREE.AudioLoader();
        audioLoader.load('sounds/music.ogg', function(buffer: THREE.AudioBuffer) {
            sound.setBuffer(buffer);
            sound.setLoop(true);
            sound.setVolume(0.5);
            sound.play();
        }, () => {}, () => {});
    }

    private onData(data: string) {
        if (!this.socket) {
            console.warn("Socket not initialized");
            return;
        }
        if (data === "WELCOME\n") {
            this.loading.setPercentage(80);
            this.socket.send("gui\n");
        } else if (data === "ok\n") {
            this.loading.setPercentage(90);
            this.socket.send(JSON.stringify({command: "map-size"}) + "\n");
        } else {
            this.loading.setPercentage(100);
            let obj : IMapSize;
            try {
                obj = JSON.parse(data);
            } catch (e) {
                console.warn("Invalid response from server: " + data);
                this.loading.setError("Impossible de se connecter au serveur Zappy");
                return;
            }
            if (obj['response-type'] === "map-size") {
                this.share.addKey("mapSize", obj.size);
                this.isSocketConnected = true;
            } else {
                this.loading.setError("Impossible de se connecter au serveur Zappy");
                console.warn("Unknown response from server: " + obj['response-type']);
            }
        }
    }

    private initSocket() {
        this.socket = new SocketCom(33333, "127.0.0.1", () => {
            this.loading.setPercentage(75);
        }, () => {
            this.loading.setError("Impossible de se connecter au serveur Zappy");
        }, (data) => {
            this.onData(data);
        });
        this.share.addKey("socket", this.socket);
    }

    public getTextureSkyBox() {
        let path = "textures/skybox/";
        let format = '.png';
        let urls = [
            path + 'right' + format,
            path + 'left' + format,
            path + 'top' + format,
            path + 'bottom' + format,
            path + 'back' + format,
            path + 'front' + format
        ];
        return urls;
    }

    public initAssets() {
        let loader = [
            this.share.getAssetsPool().loadGlTFProm('map', 'models/map/map.gltf', (obj) => {
                obj.scene.position.set(0, -3.75, 0);
                this.initSocket();
            }, (evt) => {
                this.loading.setPercentage((((evt.loaded / evt.total) * 100) * 70) / 100);
            }),
            this.share.getAssetsPool().loadGlTFProm('chicken', 'models/chicken/chicken.gltf', (obj) => {
                obj.scene.scale.set(0.7, 0.7, 0.7);
            }),
            this.share.getAssetsPool().loadCubeTextureProm('skybox', this.getTextureSkyBox())
        ];

        Promise.all(loader).then(() => {
            this.isMapLoaded = true;
        }, () => {
            this.loading.setError("Erreur lors du chargement des assets");
        });
    }

    public init() {
        this.loading.show();
        // this.isSocketConnected = true;
        // this.initSocket();
        this.initAssets();

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