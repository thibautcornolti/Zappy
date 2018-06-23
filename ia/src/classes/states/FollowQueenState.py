# coding = utf-8
from src.classes.com.Controller import controller
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.classes.ia_res.Path import Path, PathManipulator
from src.classes.ia_res.TrackableTransactions import LookTransaction, PackedTransaction, SetTransaction, \
    EmptyPathTransaction, BroadcastTransaction
from src.classes.ia_res.Vector import Vector
from src.classes.states.StateMachine import AAIState
from src.misc import my_print


class BroadcastState(object):
    pass


class FollowQueenState(AAIState):

    def __init__(self):
        super().__init__("FollowQueenState")
        self.last = Vector(-100, -100)

    def follow_dir(self, dir):
        path = Path()
        real_dir = Vector()
        if dir.x != 0:
            real_dir.x = dir.x
        else:
            real_dir.y = dir.y
        path.addPoint(real_dir, EmptyPathTransaction())
        safe_controller.execute(PathManipulator(path.generateOrder()[0], self.listen_the_queen))

    def wait_others(self):
        my_print("On the queen position, waiting for queen signal")

    def set_requested_items(self):
        transaction = PackedTransaction(self.wait_others)
        for k, v in ant.request.items():
            #my_print(k, " : ", v)
            sub_transaction = SetTransaction(k, v, lambda ok=None: None, lambda ok=None: None, lambda ok=None: None)
            transaction.addTransaction(sub_transaction)
        msg = MsgProtocol.ready_inc(ant.uuid, ant.queen.uuid)
        transaction.addTransaction(BroadcastTransaction(msg, lambda ok=None: None))
        safe_controller.execute(transaction)

    def listen_the_queen(self, _=None):
        save = None
        for msg in controller.msgQueue:
            ping = MsgProtocol.is_ping_team(msg.text)
            if ping and ping['sender'] == ant.queen.uuid:
                save = msg
        if save and (save.dir.x != self.last.x or save.dir.y != self.last.y):
            self.last = save.dir
            save = None
        if save and save.dir.x == 0 and save.dir.y == 0:
            #my_print("J'vais poser les res les gars !")
            self.set_requested_items()
        elif save:
            self.follow_dir(save.dir)
            self.last = Vector(-100, -100)
        else:
            safe_controller.execute(LookTransaction(self.listen_the_queen))

    def on_push(self, cli):
        super().on_push(cli)
        safe_controller.execute(LookTransaction(self.listen_the_queen))
