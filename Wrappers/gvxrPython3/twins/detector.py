from __future__ import annotations

from dataclasses import dataclass


@dataclass(kw_only=True)
class TwinScintillator:
    """A scintillator between the detector electronics and X-ray beam path.

    Different materials and thickness affect energy response.
    """

    material: str
    thickness: float
    energy_response: list[tuple[float, float]] | None

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinScintillator:
        material = json["material"]
        thickness = float(json["thickness"])
        energy_response = None
        if "energy_response" in json:
            energy_response = list(json["energy_response"])
            if energy_response == []:
                energy_response = None

        return TwinScintillator(
            material=material,
            thickness=thickness,
            energy_response=energy_response,
        )

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        response = "" if self.energy_response is None else " (custom response)"
        return f"Scintillator: {self.material} {self.thickness}mm{response}"


@dataclass(kw_only=True)
class TwinGain:
    """Detector gain modelled as a simple multiplier."""

    gains: list[float]

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinGain:
        if "gains" not in json:
            raise KeyError("Missing key: 'gains' not found")

        gains = list(json["gains"])

        return TwinGain(gains=gains)

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        return f"Gains: {len(self.gains)} gains"


@dataclass(kw_only=True)
class TwinADU:
    """Detector gain modelled with electronic-specific k value and ADU settings.

    The presence of k allows for custom detector gain settings.
    """

    k: float
    adus: list[float]

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinADU:
        if "k" not in json:
            raise KeyError("Missing key: 'k' not found")
        if "adus" not in json:
            raise KeyError("Missing key: 'adus' not found")
        k = float(json["k"])
        adus = list(json["adus"])

        return TwinADU(k=k, adus=adus)

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        return f"Gains: Calibrated k with {len(self.adus)} ADUs"


@dataclass(kw_only=True)
class TwinNoGain:
    """A class representing a twin with no gain support."""

    def to_json(self) -> dict:
        return {"gains": [1]}

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        return "Gain unavailable"


TwinDetectorGain = TwinADU | TwinGain | TwinNoGain


def gain_from_config(json: dict) -> TwinDetectorGain:
    """Extract a gain object from a digital twin configuration file."""
    if "k" in json:
        # Intentionally bubble up instantiation failures to ensure k, adus, and
        # gains cannot all be defined at once
        return TwinADU.from_json(json)

    try:
        g = list(json["gains"])

        if g == [1]:
            # if gains is a 1-element array with the value 1, then gain is disabled.
            return TwinNoGain()
        return TwinGain.from_json(json)

    except KeyError:
        # failed to read gains key, return no gain
        return TwinNoGain()

@dataclass(kw_only=True)
class TwinDarkfield:
    """X-ray detector darkfield."""

    bias: float
    sigma: float

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinDarkfield:
        if "bias" not in json:
            raise KeyError("Missing key: 'bias' not found")
        if "sigma" not in json:
            raise KeyError("Missing key: 'sigma' not found")

        bias = float(json["bias"])
        sigma = float(json["sigma"])

        return TwinDarkfield(bias=bias, sigma=sigma)

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        return f"Darkfield: bias: {self.bias} += {self.sigma}"


@dataclass(kw_only=True)
class TwinDetector:
    """X-ray detector specification, including resolution(s), exposures, energy response, gain, and more."""

    exposures: list[float]
    resolutions: list[tuple[int, int]]
    pixel_pitch: float
    lsf: list[float]
    scintillator: TwinScintillator
    gain: TwinDetectorGain
    darkfield: TwinDarkfield

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinDetector:
        exposures = list(json["exposures"])
        resolutions = list(json["resolutions"])
        pixel_pitch = float(json["pixel_pitch"])
        lsf = list(json["lsf"])
        scintillator = TwinScintillator.from_json(json["scintillator"])
        gain = gain_from_config(json["gain"])
        darkfield = TwinDarkfield.from_json(json["darkfield"])

        return TwinDetector(
            exposures=exposures,
            resolutions=resolutions,
            pixel_pitch=pixel_pitch,
            lsf=lsf,
            scintillator=scintillator,
            gain=gain,
            darkfield=darkfield,
        )

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        exposures = "exposures" if len(self.exposures) != 1 else "exposure"
        resolution = f"{self.resolutions[0][0]}x{self.resolutions[0][1]}"
        resolution += "" if len(self.resolutions) == 1 else f" (+{len(self.resolutions)} more)"
        lsf = "Lsf: " + "Yes" if len(self.lsf) > 0 else "No"

        return f"Detector: {resolution}, @ {self.pixel_pitch}um pixels, {len(self.exposures)} {exposures}, {lsf}, {self.scintillator}, {self.gain}, {self.darkfield}"
