# Firmware Demo Repository

This repository provides a setup for managing multiple firmware projects within a single repository, using PlatformIO for development and deployment. Each firmware project has its own deployment workflow, with a microcontroller lifecycle management system that includes GitHub Action Runners, Akri, and InfluxDB.

## Setup Repo

PlatformIO is used for firmware development, which supports managing multiple firmware projects within one repository. To deploy each firmware project independently, configure a separate workflow. Initialize each deployment pipeline by specifying the project path and device group in the workflow file.

### Example Workflow

```yaml
name: DeviceGroupCI
env:
  WORKSPACE: "./firmware/project-a"  # Path to firmware project
  DEVICEGROUP: "device-group"         # Device group name
on:
  push:
    branches:
      - main
      - "release/*"
...
```

This setup enables each firmware project to have unique workflows and paths, promoting organized project management and deployment.

## Setup Microcontroller Lifecycle Manager

### Step 1: Installing Dependencies

#### Install Github Action Runner Controller

To set up GitHub Action Runners on Kubernetes, you’ll need the GitHub Action Runner Controller which also requires Cert Manager to function properly.

```bash
kubectl apply -f https://github.com/cert-manager/cert-manager/releases/download/v1.8.2/cert-manager.yaml

helm repo add actions-runner-controller https://actions-runner-controller.github.io/actions-runner-controller
helm upgrade --install actions-runner-controller actions-runner-controller/actions-runner-controller \
  --namespace actions-runner-system --create-namespace \
  --set=authSecret.create=true \
  --set=authSecret.github_token="yourGithubToken" \
  --set=runner.statusUpdateHook.enabled=true \
  --wait
```

#### Install Akri for Device Discovery

```bash
helm repo add akri-helm-charts https://project-akri.github.io/akri/
helm install akri akri-helm-charts/akri
```

#### Install InfluxDB2

```bash
helm repo add influxdata https://helm.influxdata.com/
helm upgrade --install influx influxdata/influxdb2 -f ./helm/values/influxdb2/values.yaml
```

### Step 2: Create InfluxDB2 API-Token

After installing InfluxDB, create an API token for secure access to the database. This token will allow your services to authenticate and send data to InfluxDB.

### Step 3: Edit Values for the Microcontroller Lifecycle Manager

Edit the values file of the microcontroller lifecycle manager to configure device-specific settings. The values file can be found in the repository /helm/values/mc-lifecycle-manager/values.yaml with an example setup.

To identify the required serial numbers of the microcontrollers, run `udevadm info /dev/your-device`.

### Step 4: Install the Microcontroller Lifecycle Manager

Once configured, deploy the microcontroller lifecycle manager using Helm.

```bash
helm install mc-lifecycle-manager helm/charts/mc-lifecycle-manager -f helm/values/mc-lifecycle-manager/values.yaml
```