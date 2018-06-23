import AssetsPool from "../AssetsPool";
import {Object3D, Vector2, Vector3} from "three";
import GUIManagger from "../GUIManager";

export default class Player {
    private object: Object3D;

    constructor(assetPool: AssetsPool, position: Vector2 = new Vector2(0, 0)) {
        if (!assetPool.getGltfAssets("chicken")) {
            alert("Missing models: Chicken");
            window.location.href = "/";
        }
        this.object = assetPool.getGltfAssets("chicken").scene.clone();
        this.object.position.set(position.x, 0, position.y);
        GUIManagger.getInstance().getScene().add(this.object);
    }

    public setPosition(pos: Vector3) {
        this.object.position.set(pos.x, pos.y, pos.z);
    }

    public setRotation(rotation: Vector3) {
        this.object.rotation.set(rotation.x, rotation.y, rotation.z);
    }

    public remove() {
        GUIManagger.getInstance().getScene().remove(this.object);
    }
}