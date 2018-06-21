import * as THREE from "three"
import StateShare from "../States/StateShare"
import GUIManagger from "../GUIManager"
import {ColorKeywords} from "three";
import rosybrown = ColorKeywords.rosybrown;

export default class MainScene {
    private state: StateShare;
    private manager: GUIManagger;

    constructor(state: StateShare) {
        this.state = state;
        this.manager = GUIManagger.getInstance();
    }

    private setCamera() {
        let camera = this.manager.getCamera();

        camera.position.x = 10;
        camera.position.y = 30;
        camera.position.z = 30;
        camera.lookAt(10, 0, 10);
    }

    private setLight() {
        let intensity = 0.7;
        let light;
        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(0, 1, 0);
        light.lookAt(0, 0, 0);
        GUIManagger.getInstance().getScene().add(light);

        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(1, 1, 1);
        light.lookAt(0, 0, 0);
        GUIManagger.getInstance().getScene().add(light);

        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(-1, 1, -1);
        light.lookAt(0, 0, 0);
        GUIManagger.getInstance().getScene().add(light);

        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(0, 1, 0);
        light.lookAt(0, 20, 0);
        GUIManagger.getInstance().getScene().add(light);

        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(1, 1, 1);
        light.lookAt(0, 20, 0);
        GUIManagger.getInstance().getScene().add(light);

        light = new THREE.DirectionalLight(0xffffff, intensity);
        light.position.set(-1, 1, -1);
        light.lookAt(0, 20, 0);
        GUIManagger.getInstance().getScene().add(light);
    }

    private setMap() {
        let map = this.state.getAssetsPool().getGltfAssets("map");

        map.scene.position.set(0, -3.6, 0);
        map.scene.rotation.y = (Math.PI / 2) * 3;
        this.manager.getScene().add(map.scene);
    }

    private generateSkyBox() {
        let reflectionCube = this.state.getAssetsPool().getCubeTexture("skybox");

        reflectionCube.format = THREE.RGBFormat;
        let shader = THREE.ShaderLib[ "cube" ];
        shader.uniforms[ "tCube" ].value = reflectionCube;
        let material = new THREE.ShaderMaterial( {
            fragmentShader: shader.fragmentShader,
            vertexShader: shader.vertexShader,
            uniforms: shader.uniforms,
            depthWrite: false,
            side: THREE.BackSide
        });
        let mesh = new THREE.Mesh(new THREE.BoxGeometry(100, 100, 100), material);
        this.manager.getScene().add(mesh);
    }

    public generate() {
        let chicken1 = this.state.getAssetsPool().getGltfAssets("chicken").scene.clone();
        let chicken2 = this.state.getAssetsPool().getGltfAssets("chicken").scene.clone();

        chicken1.position.set(10, 0, 10);
        chicken1.rotateY(-Math.PI / 2);
        this.manager.getScene().add(chicken1);
        this.manager.getScene().add(chicken2);

        this.setMap();
        this.setCamera();
        this.setLight();
        this.generateSkyBox();
    }
}