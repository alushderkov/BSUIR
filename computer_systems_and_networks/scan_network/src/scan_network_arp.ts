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
function parseArpTable(arpOutput: string): NodeInfo[] {
  const lines = arpOutput.split('\n');
  const nodes: NodeInfo[] = [];

  // Регулярное выражение для поиска IP и MAC в выводе ARP
  const arpRegex = /([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\s+([0-9A-Fa-f-]+)\s+/;

  for (const line of lines) {
    const match = line.match(arpRegex);
    if (match) {
      const ip = match[1];
      const mac = match[2].replace(/-/g, ':'); // Приводим MAC к стандартному формату
      if (mac !== ':::') { // Исключаем некорректные MAC-адреса
        nodes.push({ ip, mac, hostname: 'Unknown' });
      }
    }
  }

  return nodes;
}

// Функция для получения активных узлов с фильтрацией
async function getActiveNodes(): Promise<NodeInfo[]> {
  try {
    // Получаем ARP-таблицу
    const { stdout: arpOutput } = await execAsync('arp -a');
    const nodes = parseArpTable(arpOutput);

    // Фильтруем узлы, оставляя только реальные устройства
    const filteredNodes = nodes.filter(node => isRealDevice(node.ip));

    // Получаем имена узлов через DNS
    for (const node of filteredNodes) {
      try {
        const hostnames = await reverseDnsAsync(node.ip);
        node.hostname = hostnames.length > 0 ? hostnames[0] : 'Unknown';
      } catch (error) {
        node.hostname = 'Unknown';
      }
    }

    return filteredNodes;
  } catch (error) {
    console.error('Failed to get ARP table:', error);
    return [];
  }
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

  // Получаем информацию об активных узлах
  const activeNodes = await getActiveNodes();
  console.log('Active nodes in the network:');
  activeNodes.forEach(node => {
    console.log(`IP: ${node.ip}, MAC: ${node.mac}, Hostname: ${node.hostname}`);
  });

  console.log('Network scan completed.');
}

// Запуск программы
main().catch(err => {
  console.error('An error occurred:', err);
});