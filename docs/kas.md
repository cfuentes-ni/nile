# NI Linux Embedded kas README

## Introduction

NI Linux Embedded uses [kas](https://kas.readthedocs.io/en/latest/intro.html)
for build tooling.

kas provides an extensive toolset for OpenEmbedded layer management,
using a configuration-based approach to handle layer retrieval and
bitbake setup.

It replaces our earlier git-submodule based approach, which required
extra checkout steps and was more error-prone.

## Usage

First, build the container image:

```
$ bash ./docker/create-build-nile.sh --base kas
```

Then you can use the `kas-container` script
[as directed in the kas documentation](https://kas.readthedocs.io/en/latest/userguide/kas-container.html):

```
$ ./kas-container dump kas/target-name.yml
$ ./kas-container build kas/target-name:kas/ni-org.yml
$ ./kas-container shell kas/target-name.yml
```

## Running Virtual Machines

Because `qemu` doesn't work well within the kas build container, we have
another script, `./kas-runqemu`, which has a command-line interface similar
to the [yocto upstream runqemu](https://docs.yoctoproject.org/dev-manual/qemu.html#qemu-command-line-syntax),
but takes as its first argument a kas project configuration yaml string,
which is then used to find the appropriate qemu configuration options.

```
$ ./kas-runqemu kas/target-name.yml
$ ./kas-runqemu kas/target-name.yml serial nographic
```

## Conventions

All kas configuration files should be under a `kas/` subdirectory.

(We will likely have to develop an organization under this subdirectory
as we onboard more targets.)

## Internal builders

In order to efficiently support options relevant to internal builders,
we have an `ni-org.yml` snippet that supplies additional configuration
for utilizing corporate network resources (such as the internally-hosted
source mirror).

External builders _should not_ add `ni-org.yml`.

## Building images from NI-built IPK feeds

This repository supports a two-stage workflow:

1. Stage 1 builds and exports IPK feeds.
2. Stage 2 builds images that consume those feeds.

For Stage 1 feed generation details, see `docs/feed_builds.md`.

### Enable feed-based image assembly

Include `kas/includes/image-from-feeds.yml` in your target configuration:

```yaml
header:
  version: 19
  includes:
    - kas/includes/base-config.yml
    - kas/machines/<machine>.yml
    - kas/includes/image-from-feeds.yml
```

`image-from-feeds.yml` enables:

```conf
BUILD_IMAGES_FROM_FEEDS = "1"
```

It also maps `NILE_LOCAL_FEED_URI` into the OE feed variables used by
rootfs package installation (`IPK_FEED_URIS`) and package-management feed
config generation (`PACKAGE_FEED_URIS`, `PACKAGE_FEED_ARCHS`).

### Set the feed URI

`NILE_LOCAL_FEED_URI` must point to the exported Stage 1 feed location.

- In internal CI, this is injected by pipeline infrastructure.
- For local testing, set it in local configuration, for example:

```conf
NILE_LOCAL_FEED_URI = "file:///path/to/exported/feed"
```

`NILE_LOCAL_FEED_URI` should be the base URI whose subdirectories contain
feed indexes for `all`, `${MACHINE}`, and `${TUNE_PKGARCH}`. The
`image-from-feeds.yml` include generates corresponding `IPK_FEED_URIS`
entries automatically.

If not explicitly set, `NILE_LOCAL_FEED_URI` defaults to:

```conf
NILE_LOCAL_FEED_URI = "file://${DEPLOY_DIR_IPK}"
```

For `file://` URIs used with `kas-container`, the path must exist inside
the container. Mount the host feed export path into the container and point
`NILE_LOCAL_FEED_URI` at the container-side mount path.

### Feed-only packages

If a package is only present in the feed (and does not have a build-time
provider in the local recipe graph), append it through:

```conf
IMAGE_INSTALL_NODEPS:append = " <feed-only-package>"
```

Use `IMAGE_INSTALL_NODEPS` only for intentionally feed-provided packages.

### OE-core patching model

NILE tracks required OE-core behavior through patches applied from
`kas/patches/oe-core/` via `kas/includes/base-config.yml`, including:

- `BUILD_IMAGES_FROM_FEEDS` package-manager integration.
- `PACKAGE_INSTALL_NODEPS` and `IMAGE_INSTALL_NODEPS` support.
