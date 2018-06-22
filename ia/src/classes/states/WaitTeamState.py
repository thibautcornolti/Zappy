# coding = utf-8
import enum

from src.classes.com.Controller import controller
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.misc import dup_me, my_print
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.TrackableTransactions import ForkTransaction, LookTransaction, ConnectNbrTransaction
from src.classes.states.StateMachine import AAIState, statemachine


class WaitTeamState(AAIState):

    def __init__(self):
        super().__init__("Wait")
        self._status_stack = [
            lambda: ForkTransaction(self.fork),
            lambda: ConnectNbrTransaction(self.connect_nbr),
            lambda: LookTransaction(self.wait_msg),
        ]

    def connect_nbr(self, *args):
        if len(args) != 0 and args[0] > 0:
            dup_me()
            self._status_stack.pop(0)
        self.template()

    def fork(self, *args):
        self._status_stack.pop(0)
        self.template()

    def wait_msg(self, *args):
        messages = controller.consultMessages()
        for m in messages:
            enr = MsgProtocol.is_enrolment(m)
            if enr:
                my_print("YES CEN EST UN!!!")
                exit(12093)
        #self._status_stack.pop(0)
        self.template()

    def template(self):
        if len(self._status_stack) == 0:
            statemachine.closure = lambda ok=None: statemachine.pop()
        else:
            safe_controller.execute(self._status_stack[0]())

    def on_push(self, cli):
        super().on_push(cli)
        self.template()

