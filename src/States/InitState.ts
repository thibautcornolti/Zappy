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
            }, (evt) => {
                this.loading.setPercentage((evt.loaded / evt.total) * 100);
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
        this.isSocketConnected = true;
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