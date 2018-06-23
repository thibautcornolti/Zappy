# coding = utf-8
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.classes.ia_res.TrackableTransactions import LookTransaction, BroadcastTransaction
from src.classes.com.SafeController import safe_controller
from src.classes.states.StateMachine import AAIState, statemachine
from src.misc import my_print


class WaitSlavesState(AAIState):

    def __init__(self, size, replacement_state):
        super().__init__("WaitSlavesState")
        self.size = size
        self.replacement_state = replacement_state

    def broadcast_team(self):
        safe_controller.execute(LookTransaction(self.look_for_players))

    def look_for_players(self, look):
        if look[0].count("player") == self.size:
            statemachine.closure = lambda: statemachine.replace(self.replacement_state)
        else:
            my_print("I'm here minions")
            msg = MsgProtocol.ping_team(ant.uuid)
            safe_controller.execute(BroadcastTransaction(msg, self.broadcast_team))

    def on_push(self, cli):
        super().on_push(cli)
        safe_controller.execute(LookTransaction(self.look_for_players))
