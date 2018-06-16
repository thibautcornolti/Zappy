import IState from "./IState";
import StateShare from "./StateShare";
import SockerCom from "../SocketCom";
import StateMachine from "../StateMachine";
import CoreState from "./CoreState";

export default class InitState implements IState {
    public share: StateShare;

    constructor(share: StateShare) {
        this.share = share;
    }

    public init() {
        this.share.addKey("socket", new SockerCom(33333));
    }

    public update() {
        (<StateMachine> this.share.getKey("stateMachine")).push(new CoreState(this.share));
    }
};