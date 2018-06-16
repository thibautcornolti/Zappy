import IState from "./States/IState"
import StateShare from "./States/StateShare";

export default class StateMachine {
    private states : Array<IState>;
    private share: StateShare;

    constructor(stateShare: StateShare) {
        this.states = [];
        this.share = stateShare;

        setInterval(() => {
            this.update();
        }, 10);
    }

    public push(state: IState) : StateMachine {
        if ((<any> state.init))
            (<any> state.init)();
        this.states.push(state);
        return this;
    }

    public pop() : StateMachine {
        this.states.pop();
        return this;
    }

    public update() : void {
        if (this.states.length != 0) {
            (<any> this.states[this.states.length - 1].update)();
            // if (this.states[this.states.length - 1].update)
            //     this.states[this.states.length - 1].update();
        }
    }
}