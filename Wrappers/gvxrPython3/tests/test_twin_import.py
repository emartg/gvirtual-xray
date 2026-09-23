from twins import DigitalTwin
from twins.utils import createDigitalTwin, getDigitalTwinList


def test_beam_load_tube() -> None:
    DigitalTwin.from_file("twins/models/dthe.json5")


def test_beam_load_synch_multibeam() -> None:
    DigitalTwin.from_file("twins/models/diad.json5")


def test_twin_validate() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    error, errors = diad.validate()
    assert error is False


def test_twin_invalid() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.json5")
    diad.beam.keV = 1000
    error, errors = diad.validate()

    assert error is True


def test_twin_diad_sod_sdd() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.sod = 10
    diad.odd = 20

    assert diad.sdd == 30


def test_create_digital_twin_utils() -> None:
    diad1 = createDigitalTwin(name="DIAD")
    diad2 = createDigitalTwin(id=1)
    diad3 = DigitalTwin.from_file("twins/models/diad.json5")

    assert diad1 == diad2
    assert diad1 == diad3

    dthe1 = createDigitalTwin(name="dThE")
    dthe2 = createDigitalTwin(id=0)
    dthe3 = DigitalTwin.from_file("twins/models/dthe.json5")

    assert dthe1 == dthe2
    assert dthe1 == dthe3

    assert diad1 != dthe1


def test_list_twins() -> None:
    twins = getDigitalTwinList()

    assert len(twins) > 1


def test_list_beams() -> None:
    twin = createDigitalTwin(name="dthe")
    for beam_name, beam in twin.specification.beams.items():
        print("Supported beams:")
        print(f" - {beam_name}: {beam.beam_type}")
