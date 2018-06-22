# coding = utf-8
from ia.src.classes.com.Controller import CmdCost
from ia.src.classes.ia_res.Vector import Vector


class Transaction:
    def __init__(self, estimated_time, end):
        self.estimated_time = estimated_time
        self._end = end

    def execute(self):
        raise NotImplementedError("Invalid Transaction")

    def end(self, *args, **kwargs):
        self._end(*args, **kwargs)

    def get_estimated_time(self):
        return self.estimated_time

    def __repr__(self):
        return repr("TransactionTime: %d" % self.estimated_time)


class TrackableTransaction(Transaction):

    def execute(self):
        raise NotImplementedError("Invalid Transaction")

    def __init__(self, estimated_time, end, position):
        super().__init__(estimated_time, end)
        self.position = position
