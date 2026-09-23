from __future__ import annotations

from dataclasses import dataclass


@dataclass(kw_only=True)
class XYZRange:
    x: tuple[float, float, float | None] | None
    y: tuple[float, float, float | None] | None
    z: tuple[float, float, float | None] | None

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> XYZRange:
        x = list(json["x"])
        y = list(json["y"])
        z = list(json["z"])

        if len(x) == 0:
            x = None
        elif len(x) > 1 and len(x) < 3:
            x = (x[0], x[1], None)
        else:
            x = (x[0], x[1], x[2])

        if len(y) == 0:
            y = None
        elif len(y) > 1 and len(y) < 3:
            y = (y[0], y[1], None)
        else:
            y = (y[0], y[1], y[2])

        if len(z) == 0:
            z = None
        elif len(z) > 1 and len(z) < 3:
            z = (abs(z[0]), abs(z[1]), None)
        else:
            z = (abs(z[0]), abs(z[1]), z[2])

        return XYZRange(x=x, y=y, z=z)

    def get_default_position(self) -> tuple[float, float, float]:
        # Position in [mm],
        # Y values are absolute distance in mm away from sample at 0, 0, 0
        default_x = 0
        default_y = 500
        default_z = 0

        # Use default value, or midpoint between min-max if default is not set.
        if self.x is not None:
            default_x = self.x[2] if self.x[2] is not None else self.x[0] + (self.x[1] - self.x[0]) / 2

        if self.y is not None:
            default_y = self.y[2] if self.y[2] is not None else self.y[0] + (self.y[1] - self.y[0]) / 2

        if self.z is not None:
            default_z = self.z[2] if self.z[2] is not None else self.z[0] + (self.z[1] - self.z[0]) / 2

        # Y is always positive.
        default_y = abs(default_y)

        return (default_x, default_y, default_z)


@dataclass(kw_only=True)
class TwinStage:
    source: XYZRange
    detector: XYZRange
    static_sdd: bool

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinStage:
        source = XYZRange.from_json(json["source"])
        detector = XYZRange.from_json(json["detector"])
        static_sdd = bool(json["static_sdd"])

        return TwinStage(
            source=source,
            detector=detector,
            static_sdd=static_sdd,
        )
