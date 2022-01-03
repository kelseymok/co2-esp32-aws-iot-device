#!/bin/bash
script_dir=$(cd "$(dirname "$0")" ; pwd -P)

configFile="${script_dir}/device/app/data/config.json"
dataPath="${script_dir}/device/app/data"
pushd "${script_dir}/infrastructure" > /dev/null

  curl -o "${dataPath}/ca.crt" https://www.amazontrust.com/repository/AmazonRootCA1.pem
  terraform output -raw device-certificate > "${dataPath}/device.pem.crt"
  terraform output -raw device-private-key > "${dataPath}/private-key.pem"

  rm -f $configFile
  touch $configFile
  read -p "Wifi SSID: " wifi_ssid
  read -p "Wifi password: " wifi_pw

  aws_iot_endpoint=$(terraform output -raw iot-endpoint-address | sed -e 's/.iot/-ats.iot/g')
  thing_name=$(terraform output -raw thing-name)
  jq -n --arg address "${aws_iot_endpoint}" '.["aws_iot_endpoint"]=$address' \
    | jq --arg thing "${thing_name}" '.["thing_name"]=$thing' \
    | jq --arg subtopic "${thing_name}/pub" '.["aws_iot_publish_topic"]=$subtopic' \
    | jq --arg wifissid "${wifi_ssid}" '.["wifi_ssid"]=$wifissid' \
    | jq --arg wifipw "${wifi_pw}" '.["wifi_password"]=$wifipw' \
    > "${configFile}"


popd > /dev/null
