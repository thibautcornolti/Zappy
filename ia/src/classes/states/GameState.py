# coding = utf-8
from src.classes.ia_res.Ant import ant
from src.classes.states.LevelUpHandlingState import LevelUpHandlingState
from src.classes.states.StateMachine import AState, statemachine
from src.misc import my_print


class GameState(AState):

    def __init__(self):
        super().__init__("Game")

    def check(self):
        if ant.lvl < 8:
            statemachine.closure = lambda: statemachine.push(LevelUpHandlingState())
        else:
            my_print("END OF THE GAME")
            exit(0)

    def on_push(self, cli):
        super().on_push(cli)
        self.check()

    def popped_over(self):
        super().popped_over()
        self.check()

