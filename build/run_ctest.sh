
# Not running on Manylinux
if ! command -v manylinux-interpreters &> /dev/null
then
    echo Not running on Manylinux
    ctest -D NightlyUpdate -D NightlyStart -D NightlyConfigure -D NightlyBuild -D NightlyTest -D NightlyCoverage  -D NightlyMemCheck -D NightlySubmit

# Run on manylinux
else
    echo Run on Manylinux
    ctest -D NightlyStart -D NightlyConfigure -D NightlyBuild -D NightlyTest -D NightlyCoverage  -D NightlyMemCheck -D NightlySubmit
fi
