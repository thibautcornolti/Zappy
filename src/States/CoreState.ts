import IState from "./IState"
import GUIManager from "../GUIManager"
import StateShare from "./StateShare";

export default class CoreState implements IState {
    private manager: GUIManager;
    private state: StateShare;

    public constructor(state: StateShare) {
        this.manager = GUIManager.getInstance();
        this.state = state;
    }

    update() {
        // console.log(this.manager.getCamera());
    }
}