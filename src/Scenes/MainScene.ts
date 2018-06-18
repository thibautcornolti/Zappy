import * as THREE from "three"
import StateShare from "../States/StateShare"
import GUIManagger from "../GUIManager"

export default class MainScene {
    private state: StateShare;
    private manager: GUIManagger;

    constructor(state: StateShare) {
        this.state = state;
        this.manager = GUIManagger.getInstance();
    }

    private setCameraPosition() {
        let camera = this.manager.getCamera();

        camera.position.z = 30;
        camera.position.x = 10;
        camera.position.y = 30;
        camera.lookAt(10, 0, 10);
    }

    private setLight() {
        let light = new THREE.AmbientLight(0xffffff, 0.7);
        this.manager.getScene().add(light);
        light = new THREE.DirectionalLight(0xffffff, 0.7);
        light.position.set(0, 1, 0);
        light.lookAt(0, 0, 0);
        this.manager.getScene().add(light);
    }

    public generate() {
        let map = this.state.getAssetsPool().getAssets("map");

        map.rotation.y = (Math.PI / 2) * 3;
        this.manager.getScene().add(map);

        this.setCameraPosition();
        this.setLight();
    }
}