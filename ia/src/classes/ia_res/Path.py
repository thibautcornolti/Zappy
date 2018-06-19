# coding = utf-8
import math
from ia.src.classes.ia_res.Ant import Vector
from ia.src.classes.com.Controller import Cmd

def vecSize(vector):
    return math.sqrt(vector.x * vector.x + vector.y * vector.y)

def normalize(vector):
    size = vecSize(vector)
    if size == 0:
        return Vector()
    return Vector(vector.x / size, vector.y / size)

class Path(object):

    def __init__(self):
        self.go = list()
        self.ret = list()

        self.points = list()

    def addPoint(self, pos, cmd):
        self.points.append((pos, cmd))

    def addConePoint(self, idx, cmd):
        pos = None
        min_val = 0
        max_val = 0
        deep = 0
        while pos is None:
            if min_val <= idx <= max_val:
                middle = min_val + (max_val - min_val) // 2
                x = idx - middle
                pos = Vector(x, deep)
            deep += 1
            min_val = max_val + 1
            max_val = min_val + deep * 2
        self.addPoint(pos, cmd)

    def _generateReverse(self):
        return list()

    def _alignX(self, look, src_point, dest_point):
        wsrc_point = Vector(src_point.x, 0)
        wdest_point = Vector(dest_point.x, 0)
        dir_vec = normalize(Vector(wdest_point.x - wsrc_point.x, wdest_point.y - wsrc_point.y))
        nlook = normalize(look)
        new_look = dir_vec
        moves = list()
        if nlook.y == 0 and nlook.x == -dir_vec.x and nlook.x != 0:
            moves += [Cmd.Left, Cmd.Left]
        elif nlook.y != 0 and dir_vec.x == -nlook.y:
            moves += [Cmd.Left]
        elif nlook.y != 0 and dir_vec.x == nlook.y:
            moves += [Cmd.Right]
        else:
            new_look = look
        size = vecSize(Vector(wdest_point.x - wsrc_point.x, wdest_point.y - wsrc_point.y))
        for i in range(int(size)):
            moves += [Cmd.Forward]
        return new_look, moves, Vector(dest_point.x, src_point.y)

    def _alignY(self, look, src_point, dest_point):
        wsrc_point = Vector(0, src_point.y)
        wdest_point = Vector(0, dest_point.y)
        dir_vec = normalize(Vector(wdest_point.x - wsrc_point.x, wdest_point.y - wsrc_point.y))
        nlook = normalize(look)
        new_look = dir_vec
        moves = list()
        if nlook.x == 0 and nlook.y == -dir_vec.y and nlook.y != 0:
            moves += [Cmd.Left, Cmd.Left]
        elif nlook.x != 0 and dir_vec.y == nlook.x:
            moves += [Cmd.Left]
        elif nlook.x != 0 and dir_vec.y == -nlook.x:
            moves += [Cmd.Right]
        else:
            new_look = look
        size = vecSize(Vector(wdest_point.x - wsrc_point.x, wdest_point.y - wsrc_point.y))
        for i in range(int(size)):
            moves += [Cmd.Forward]
        return new_look, moves, Vector(src_point.x, dest_point.y)

    def _calcMove(self, look, src_point, dest_point):
        if src_point.x == dest_point.x and src_point.y == dest_point.y:
            print([])
            return look, list()
        xylook = look
        xysrc_point = src_point
        xyactions_list = list()
        xylook, new_moves, xysrc_point = self._alignX(xylook, xysrc_point, dest_point)
        xyactions_list += new_moves
        xylook, new_moves, xysrc_point = self._alignY(xylook, xysrc_point, dest_point)
        xyactions_list += new_moves
        yxlook = look
        yxsrc_point = src_point
        yxactions_list = list()
        yxlook, new_moves, yxsrc_point = self._alignY(yxlook, yxsrc_point, dest_point)
        yxactions_list += new_moves
        yxlook, new_moves, yxsrc_point = self._alignX(yxlook, yxsrc_point, dest_point)
        yxactions_list += new_moves

        if len(yxactions_list) < len(xyactions_list):
            print("YX", yxactions_list)
            return yxlook, yxactions_list
        print("YX", xyactions_list)
        return xylook, xyactions_list

    def _generateGoOpti(self, path, moves, points, current_point, current_look):
        if len(points) == 0:
            len_go = len(self.go)
            if len(moves) < len_go or len_go == 0:
                self.go = moves
            return
        save = list(points)
        move_save = list(moves)
        for i in range(len(points)):
            save.pop(i)
            path.insert(0, points[i][0])
            new_look, new_moves = self._calcMove(current_look, current_point, points[i][0])
            moves += new_moves + [points[i][1]]
            self._generateGoOpti(path, moves, save, points[i][0], new_look)
            moves = list(move_save)
            path.pop(0)
            save = list(points)

    def _generateGoOrder(self, points, current_point, current_look):
        actions_list = list()
        for point in points:
            current_look, actions = self._calcMove(current_look, current_point, point[0])
            actions_list += actions + [point[1]]
            current_point = point[0]
        self.go = actions_list

    def generateOpti(self):
        self._generateGoOpti(list(), list(), self.points, Vector(), Vector(0, 1))
        self._generateReverse()
        ret = self.go, self.ret
        self.go = list()
        self.ret = list()
        return ret

    def generateOrder(self):
        self._generateGoOrder(self.points, Vector(), Vector(0, 1))
        self._generateReverse()
        ret = self.go, self.ret
        self.go = list()
        self.ret = list()
        return ret
