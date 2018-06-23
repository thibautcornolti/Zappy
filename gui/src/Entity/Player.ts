import AssetsPool from "../AssetsPool";
import { Object3D, Vector2, Vector3 } from "three";
import GUIManagger from "../GUIManager";

export default class Player {
    private object: Object3D;
    private dest: Vector3;
    private speedX: number;
    private speedZ: number;
    private destRot: Vector3;
    private speedRot: number;
    private timeInterval: any;
    private timeIntervalRot: any;

    constructor(assetPool: AssetsPool, position: Vector2 = new Vector2(0, 0)) {
        if (!assetPool.getGltfAssets("chicken")) {
            alert("Missing models: Chicken");
            window.location.href = "/";
        }
        this.timeInterval = null;
        this.timeIntervalRot = null;
        this.speedX = 0;
        this.speedZ = 0;
        this.speedRot = 0;
        this.object = assetPool.getGltfAssets("chicken").scene.clone();
        this.dest = new Vector3(position.x, 0, position.y);
        this.destRot = new Vector3(position.x, 0, position.y);
        this.object.position.set(position.x, 0, position.y);
        GUIManagger.getInstance().getScene().add(this.object);
    }

    public setPosition(pos: Vector3) {
        let count = 5;
        if (this.timeInterval) {
            clearInterval(this.timeInterval);
            this.timeInterval = null;
        }
        this.dest = pos
        this.speedX = Math.abs(pos.x - this.object.position.x) / count;
        this.speedZ = Math.abs(pos.z - this.object.position.z) / count;
        this.timeInterval = setInterval(() => {
            count--;
            if (!count && this.timeInterval) {
                clearInterval(this.timeInterval);
                this.timeInterval = null;
            }
            let newX = this.object.position.x;
            let newZ = this.object.position.z;
            if (this.dest.x > this.object.position.x)
                newX += this.speedX;
            else if (this.dest.x < this.object.position.x)
                newX -= this.speedX;
            if (this.dest.z > this.object.position.z)
                newZ += this.speedZ;
            else if (this.dest.z < this.object.position.z)
                newZ -= this.speedZ;
            this.object.position.set(newX, this.dest.y, newZ);
        }, 25);
    }

    public setRotation(rotation: Vector3) {
        let count = 5;
        if (this.timeIntervalRot) {
            clearInterval(this.timeIntervalRot);
            this.timeIntervalRot = null;
        }
        this.destRot = rotation
        this.speedRot = Math.abs(rotation.y - this.object.rotation.y) / count;
        this.timeIntervalRot = setInterval(() => {
            count--;
            if (!count && this.timeIntervalRot) {
                clearInterval(this.timeIntervalRot);
                this.timeIntervalRot = null;
            }
            let newRot = this.object.rotation.y;
            if (this.destRot.y > this.object.rotation.y)
                newRot += this.speedRot;
            else if (this.destRot.y < this.object.rotation.y)
                newRot -= this.speedRot;
            this.object.rotation.set(this.destRot.x, newRot, this.destRot.z);
        }, 25, 0);
    }

    public remove() {
        GUIManagger.getInstance().getScene().remove(this.object);
    }
}