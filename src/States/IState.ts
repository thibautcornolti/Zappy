import StateMachine from "../StateMachine";

export default interface IState {
    init?() : void,
    update?() : void,
    draw?() : void
}