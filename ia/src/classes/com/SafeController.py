# coding = utf-8
from src.classes.com.Controller import Resources
from src.classes.ia_res.Ant import ant
from src.classes.ia_res.TrackableTransactions import InventoryTransaction
from src.classes.states.StateMachine import statemachine
from src.misc import my_log, my_print


class SafeController(object):

    def __init__(self):
        self.rollback = True
        self.floor = None
        self.save = None
        self.endTransaction = None
        self.safe = True

    def clear_transaction(self, *args, **kwargs):
        my_log("End Transaction : ", self.save)
        endTransa = self.endTransaction
        self.save = None
        self.floor = None
        self.endTransaction = None
        endTransa(*args, **kwargs)

    def clear_state(self, cli):
        del cli

        def reset_trans():
            statemachine.block_trans_detect = False

        statemachine.closure = reset_trans
        self.safe = True
        self.safe_exec(self.save, self.floor, self.rollback)

    def estimate_food(self, inventory):
        from src.classes.states.SeekItemsState import SeekItemsState

        # if Resources.Food not in ant.inventory or ant.inventory[Resources.Food] != inventory[Resources.Food]:
        #     my_print("Food lvl : ", inventory[Resources.Food])
        ant.inventory = inventory
        if (inventory[Resources.Food] < self.save.get_estimated_time() / 126 or inventory[Resources.Food] < self.floor) and self.floor >= 0:
            my_print("\tEMERGENCY MOD suspending : ", self .save, " at food lvl : ", inventory[Resources.Food])
            statemachine.block_trans_detect = True
            self.safe = False
            state = SeekItemsState({Resources.Food: int(self.save.get_estimated_time() / 126 + 2 * int(self.floor))}, self.rollback)
            state.on_pop = self.clear_state
            statemachine.push(state)
        else:
            self.endTransaction = self.save.end
            self.save.end = self.clear_transaction
            self.save.execute()

    def safe_exec(self, transaction, floor, rollback):
        self.save = transaction
        self.floor = floor
        self.rollback = rollback
        InventoryTransaction(self.estimate_food).execute()

    def execute(self, transaction, floor=15, rollback=True):
        my_log("Start Transaction : ", transaction)
        if self.save and self.safe:
            raise Exception("Invalid Concurrent transaction : {} vs {}".format(self.save.__repr__(), transaction.__repr__()))
        if self.safe:
            self.safe_exec(transaction, floor, rollback)
        else:
            transaction.execute()


safe_controller = SafeController()
