import * as THREE from "three"

export default class GUIManagger {
    private static _instance : GUIManagger;
    private scene: THREE.Scene;
    private camera: THREE.PerspectiveCamera;
    private renderer: THREE.WebGLRenderer;


    private constructor() {
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth/window.innerHeight, 0.1, 1000);
        this.renderer = new THREE.WebGLRenderer();

        this.camera.position.z = 30; //TODO Remove this
        this.camera.position.y = 40;
        this.camera.lookAt(0, 0, 10);
        let light = new THREE.AmbientLight(0xffffff, 0.7);
        this.scene.add(light);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        document.body.appendChild(this.renderer.domElement);

        window.addEventListener('resize', this.onWindowResize, false);
        // requestAnimationFrame(this.animate);
    }

    // private animate = () => {
    //     this.renderer.render(this.scene, this.camera);
    // };

    private onWindowResize : EventListenerOrEventListenerObject = () => {
        // console.log('ok');
        this.camera.aspect = window.innerWidth / window.innerHeight;
        this.camera.updateProjectionMatrix();
        this.renderer.setSize(window.innerWidth, window.innerHeight);
    };

    public static getInstance() : GUIManagger {
        return this._instance || (this._instance = new this());
    }

    public getRenderer() : THREE.WebGLRenderer {
        return this.renderer;
    }

    public getScene() : THREE.Scene {
        return this.scene;
    }

    public getCamera() : THREE.Camera {
        return this.camera;
    }
}