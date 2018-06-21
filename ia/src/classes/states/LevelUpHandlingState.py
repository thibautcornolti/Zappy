# coding = utf-8
import enum
from src.classes.com.Controller import requirement
from src.classes.ia_res.Ant import ant
from src.classes.states.StateMachine import AAIState, statemachine


class Status(enum.Enum):
    StandBy = 0
    Farming = 1
    Casting = 2


class LevelUpHandlingState(AAIState):

    def __init__(self):
        super().__init__("LevelUpHandler")
        self.lvl = ant.lvl + 1

    def on_push(self, cli):
        from src.classes.states.LvlAloneState import LevelUpAlone
        super().on_push(cli)
        if requirement[self.lvl][0] == 1:
            statemachine.closure = lambda: statemachine.push(LevelUpAlone())
        else:
            statemachine.closure = lambda: exit(84)

    def popped_over(self):
        super().popped_over()
        statemachine.closure = lambda: statemachine.pop()
