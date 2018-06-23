# coding = utf-8
from src.classes.com.Controller import controller
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.classes.ia_res.Path import Path, PathManipulator
from src.classes.ia_res.TrackableTransactions import LookTransaction, PackedTransaction, SetTransaction
from src.classes.ia_res.Vector import Vector
from src.classes.states.StateMachine import AAIState
from src.misc import my_print


class FollowQueenState(AAIState):

    def __init__(self):
        super().__init__("FollowQueenState")

    def follow_dir(self, dir):
        path = Path()
        real_dir = Vector()
        if dir.x != 0:
            real_dir.x = dir.x
        else:
            real_dir.y = dir.y
        path.addPoint(real_dir, lambda ok=None: None)
        safe_controller.execute(PathManipulator(path, self.listen_the_queen))

    def wait_others(self):
        my_print("On the queen position, waiting for queen signal")

    def set_requested_items(self):
        transaction = PackedTransaction(self.wait_others)
        for k, v in ant.request.items():
            sub_transaction = SetTransaction(k, v, lambda ok=None: None, lambda ok=None: None, lambda ok=None: None)
            transaction.addTransaction(sub_transaction)
        safe_controller.execute(transaction)

    def listen_the_queen(self, _):
        save = None
        for msg in controller.msgQueue:
            ping = MsgProtocol.is_ping_team(msg.text)
            if ping and ping['sender'] == ant.queen.uuid:
                save = ping
        if save and save.dir.x == 0 and save.dir.y == 0:
            self.set_requested_items()
        elif save:
            self.follow_dir(save.dir)
        else:
            safe_controller.execute(LookTransaction(self.listen_the_queen))

    def on_push(self, cli):
        super().on_push(cli)
        safe_controller.execute(LookTransaction(self.listen_the_queen))
