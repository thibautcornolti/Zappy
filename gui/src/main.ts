import StateMachine from "./StateMachine";
import StateShare from "./States/StateShare";
import InitState from "./States/InitState";
import AssetsPool from "./AssetsPool";

function main() {
    let assetsPool = new AssetsPool();
    let stateShare = new StateShare();
    let stateMachine = new StateMachine(stateShare);
    stateShare.addKey("stateMachine", stateMachine);
    stateShare.addKey("assetsPool", assetsPool);

    stateMachine.push(new InitState(stateShare));
}

main();