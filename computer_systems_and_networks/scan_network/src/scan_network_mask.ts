import { exec } from 'child_process';
import os from 'os';
import dns from 'dns';
import { promisify } from 'util';

// Преобразуем callback-функции в Promise для удобства
const execAsync = promisify(exec);
const reverseDnsAsync = promisify(dns.reverse);

// Интерфейс для хранения информации об узле
interface NodeInfo {
  ip: string;
  mac: string;
  hostname: string;
}

// Функция для проверки, является ли IP-адрес реальным устройством
function isRealDevice(ip: string): boolean {
  // Исключаем широковещательные, multicast и другие служебные адреса
  const excludedPatterns = [
    /\.255$/, // Широковещательные адреса
    /^224\./, // Multicast-адреса
    /^239\./, // Multicast-адреса
    /^255\.255\.255\.255$/, // Локальный широковещательный адрес
  ];

  return !excludedPatterns.some(pattern => pattern.test(ip));
}

// Функция для получения MAC-адреса из вывода команды `arp`
function parseMacAddress(arpOutput: string): string {
  const macRegex = /([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})/;
  const match = arpOutput.match(macRegex);
  return match ? match[0] : 'Unknown';
}

// Функция для получения диапазона IP-адресов на основе маски подсети
function calculateIpRange(ip: string, netmask: string): string[] {
  const ipParts = ip.split('.').map(Number);
  const netmaskParts = netmask.split('.').map(Number);

  const network = ipParts.map((part, index) => part & netmaskParts[index]);
  const broadcast = network.map((part, index) => part | (~netmaskParts[index] & 0xff));

  const ipRange: string[] = [];
  for (let i = network[3] + 1; i < broadcast[3]; i++) {
    const ipAddress = `${network[0]}.${network[1]}.${network[2]}.${i}`;
    if (isRealDevice(ipAddress)) {
      ipRange.push(ipAddress);
    }
  }

  return ipRange;
}

// Функция для сканирования сети с перебором адресов
async function scanNetwork(ipRange: string[], localIp: string, localMac: string): Promise<NodeInfo[]> {
  const nodes: NodeInfo[] = [];

  // Параллельное выполнение ping-запросов
  const pingPromises = ipRange.map(async (ip) => {
    try {
      await execAsync(`ping -n 1 ${ip}`);
      return ip;
    } catch (error) {
      return null;
    }
  });

  const activeIps = (await Promise.all(pingPromises)).filter(ip => ip !== null) as string[];

  // Параллельное выполнение ARP-запросов и DNS-запросов
  const nodePromises = activeIps.map(async (ip) => {
    try {
      // Если это локальный IP, используем локальный MAC-адрес
      let mac = ip === localIp ? localMac : parseMacAddress((await execAsync(`arp -a ${ip}`)).stdout);

      // Если MAC-адрес неизвестен, выполняем дополнительный ARP-запрос
      if (mac === 'Unknown') {
        console.warn(`MAC-адрес для узла ${ip} неизвестен. Выполняю дополнительный ARP-запрос...`);
        await execAsync(`ping -n 1 ${ip}`); // Пинг для обновления ARP-таблицы
        const { stdout: arpOutput } = await execAsync(`arp -a ${ip}`);
        mac = parseMacAddress(arpOutput);
      }

      // Если MAC-адрес всё ещё неизвестен, выводим предупреждение
      if (mac === 'Unknown') {
        console.warn(`MAC-адрес для узла ${ip} остаётся неизвестным. Возможно, устройство не отвечает на ARP-запросы.`);
      }

      // Получение имени узла через DNS
      const hostnames = await reverseDnsAsync(ip);
      const hostname = hostnames.length > 0 ? hostnames[0] : 'Unknown';

      return { ip, mac, hostname };
    } catch (error) {
      return null;
    }
  });

  const discoveredNodes = (await Promise.all(nodePromises)).filter(node => node !== null) as NodeInfo[];

  nodes.push(...discoveredNodes);
  return nodes;
}

// Функция для получения информации о текущем компьютере
function getLocalNodeInfo(): NodeInfo[] {
  const interfaces = os.networkInterfaces();
  const localNodes: NodeInfo[] = [];

  for (const interfaceName in interfaces) {
    const addresses = interfaces[interfaceName];
    if (!addresses) continue;

    for (const address of addresses) {
      if (address.family === 'IPv4' && !address.internal) {
        localNodes.push({
          ip: address.address,
          mac: address.mac,
          hostname: os.hostname(),
        });
      }
    }
  }

  return localNodes;
}

// Основная функция
async function main() {
  console.log('Starting network scan...');

  // Получаем информацию о текущем компьютере
  const localNodes = getLocalNodeInfo();
  console.log('Local node info:');
  localNodes.forEach(node => {
    console.log(`IP: ${node.ip}, MAC: ${node.mac}, Hostname: ${node.hostname}`);
  });

  // Получаем диапазон IP-адресов для сканирования
  const interfaces = os.networkInterfaces();
  for (const interfaceName in interfaces) {
    const addresses = interfaces[interfaceName];
    if (!addresses) continue;

    for (const address of addresses) {
      if (address.family === 'IPv4' && !address.internal) {
        const ipRange = calculateIpRange(address.address, address.netmask);
        console.log(`Scanning network ${address.address}/${address.netmask}...`);

        // Сканируем сеть
        const nodes = await scanNetwork(ipRange, address.address, address.mac);
        console.log('Discovered nodes:');
        nodes.forEach(node => {
          console.log(`IP: ${node.ip}, MAC: ${node.mac}, Hostname: ${node.hostname}`);
        });
      }
    }
  }

  console.log('Network scan completed.');
}

// Запуск программы
main().catch(err => {
  console.error('An error occurred:', err);
});