# coding = utf-8
import itertools
from collections import Counter

from src.classes.com.Controller import requirement, controller
from src.classes.com.SafeController import safe_controller
from src.classes.ia_res.Ant import ant, mates
from src.classes.ia_res.MsgProtocol import MsgProtocol
from src.classes.ia_res.TrackableTransactions import PackedTransaction, BroadcastTransaction, LookTransaction
from src.classes.states.SeekTeamState import SeekTeamState
from src.classes.states.StateMachine import AAIState, statemachine
from src.classes.states.WaitTeamState import WaitTeamState
from src.misc import my_print


def split_seq(seq, p):
    newseq = []
    n = len(seq) / p
    r = len(seq) % p
    b, e = 0, int(n + min(1, r))
    for i in range(p):
        newseq.append(seq[b:e])
        r = int(max(0, r - 1))
        b, e = int(e), int(e + n + min(1, r))
    return newseq


class QueenState(AAIState):

    def __init__(self):
        super().__init__("QueenState")

    def on_push(self, cli):
        super().on_push(cli)
        statemachine.closure = lambda: statemachine.push(SeekTeamState())

    def resource_repart(self):
        my_print("repartition des tâches : ")
        full_list = list(itertools.chain(*[[k.value] * v for k, v in requirement[ant.lvl + 1][1].items()]))
        full_list = split_seq(full_list, len(mates))
        full_dict = [dict(Counter(e)) for e in full_list]
        transaction = PackedTransaction(self.wait_answers)
        for m, items in zip(mates, full_dict):
            my_print(" - ", m.uuid, " : ", items)
            m.inventory = items
            msg = MsgProtocol.seek_slave(ant.uuid, m.uuid, items)
            transaction.addTransaction(BroadcastTransaction(msg, lambda ok=None: None))
        safe_controller.execute(transaction)

    def wait_answers(self, *args):
        for m in controller.msgQueue:
            my_print(m.text)
            end = MsgProtocol.is_seek_end(m.text)
            if end and end['sender'] in (m.uuid for m in mates):
                my_print(end['sender'], " finished all the tasks")
                m.inventory = dict()

        end = True
        for mate in mates:
            if len(mate.inventory):
                end = False
                break

        if end:
            msg = MsgProtocol.meet_ants(ant.uuid, [m.uuid for m in mates])
            transaction = BroadcastTransaction(msg, lambda: exit(0))
        else:
            transaction = LookTransaction(self.wait_answers)
        safe_controller.execute(transaction)

    def popped_over(self):
        super().popped_over()
        if not ant.is_queen:
            statemachine.closure = lambda: statemachine.replace(WaitTeamState())
        else:
            my_print("IM THE QUEEN !!", statemachine._stack)
            self.resource_repart()


if __name__ == "__main__":
    my_print(split_seq(range(10), 3))
    my_print(split_seq(range(11), 3))
    my_print(split_seq(range(1), 2))
    my_print(split_seq(range(10), 5))
    my_print(split_seq(range(10), 2))

