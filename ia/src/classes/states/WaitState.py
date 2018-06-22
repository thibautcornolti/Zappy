# coding = utf-8
import re

from src.classes.com.Controller import controller
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.Ant import mates
from src.classes.states.StateMachine import AAIState, statemachine


class WaitState(AAIState):

    def __init__(self):
        super().__init__("Wait")

    def check_msg(self):
        msg = controller.consultMsg()
        # if not msg:
        #     safe_controller.look(self.check_msg)
        # elif re.search("^[0-9a-z]* Je recrute bro !!$", msg.text):
        #     uuid = re.search("^[0-9a-z]")
        #     mate = mates.get_mate(uuid)
        #     if mate:
        #         next(mate)

    def on_push(self, cli):
        super().on_push(cli)
        safe_controller.look(self.check_msg)
